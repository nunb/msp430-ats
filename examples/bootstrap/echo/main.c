/** This file is in the public domain.
 *
 * This program initializes the platform then spins toggling the first
 * LED while it echos characters received on the console port back to
 * the console port.
 *
 * Its role is primarily as a sanity check when bootstrapping a new
 * platform: the final step is to support interactivity.
 *
 * For diagnostics, LED0 blinks at 1Hz while the program is running.
 * LED1 is set when a character is received, and cleared when it is
 * transmitted.
 *
 * @author Peter A. Bigot <bigotp@acm.org>
 * @homepage http://github.com/pabigot/freertos-mspgcc
 * @date 2012
 *
 */

/* Include the generic platform header.  This assumes that
 * #BSP430_PLATFORM_EXP430FR5739 (or another marker that identifies a
 * platform BSP430 supports) has been defined for the preprocessor.
 * If not, you may include the platform-specific version,
 * e.g. <bsp430/platform/exp430g2.h>. */
#include <bsp430/platform.h>

/* We're going to use LEDs.  Enable them. */
#include <bsp430/utility/led.h>

/* We want to know the nominal clock speed so we can delay. */
#include <bsp430/clock.h>

/* We require console support as verified by <bsp430/platform.h> */
#ifndef BSP430_CONSOLE
#error No console UART PERIPH handle has been defined
#endif /* BSP430_CONSOLE */
#include <bsp430/utility/console.h>

/* The buffer in which pending characters are placed.  A negative
 * value indicates the space is available. */
int to_tx = -1;

static int
rx_callback (const struct sBSP430halISRCallbackVoid * cb,
             void * context)
{
  hBSP430halSERIAL device = (hBSP430halSERIAL)context;
  vBSP430ledSet(1, 1);

  /* If there's space, queue the received character for
   * transmission. */
  if (0 > to_tx) {
    to_tx = device->rx_byte;
    vBSP430serialWakeupTransmit_ni(device);
  }

  /* There are no flags to be communicated back to the ISR */
  return 0;
}

struct sBSP430halISRCallbackVoid rx_entry = {
  .callback = rx_callback
};

static int
tx_callback (const struct sBSP430halISRCallbackVoid * cb,
             void * context)
{
  hBSP430halSERIAL device = (hBSP430halSERIAL)context;
  int rv = 0;

  vBSP430ledSet(1, 0);

  /* If there's a pending character, store it in the transmission
   * buffer and inform the ISR that it's there.  Clear the local state
   * so it doesn't get transmitted twice. */
  if (0 <= to_tx) {
    device->tx_byte = to_tx;
    to_tx = -1;
    rv |= BSP430_HAL_ISR_CALLBACK_BREAK_CHAIN;
  }

  /* If we permitted multiple characters to be queued, we might want
   * to leave the interrupt enabled so this would be reinvoked when
   * the one we're about to submit got out the door. But we don't, so
   * we don't. */
  rv |= BSP430_HAL_ISR_CALLBACK_DISABLE_INTERRUPT;
  return rv;
}

struct sBSP430halISRCallbackVoid tx_entry = {
  .callback = tx_callback
};

void main ()
{
  hBSP430halSERIAL con;

  /* First thing you do in main is configure the platform. */
  vBSP430platformInitialize_ni();

  /* Now initialize the LEDs */
  vBSP430ledInitialize_ni();

  /* Configure the echo using the standard console handle */
  (void)iBSP430consoleInitialize();
  con = hBSP430console();
  if (NULL == con) {
    return;
  }
  cprintf("Basic console up\n");

  /* Place the console into hold while configuring the callbacks. */
  cprintf("Initializing for interrupt-driven I/O\nType to see echo: ");
  (void)iBSP430serialSetHold_ni(con, 1);

  /* Add the local handlers to the head of the chain.  The previous
   * ones are probably NULL. */
  rx_entry.next = con->rx_callback;
  con->rx_callback = &rx_entry;
  tx_entry.next = con->tx_callback;
  con->tx_callback = &tx_entry;

  /* Releasing the hold will enable the interrupt-driven
   * communications. */
  if (0 != iBSP430serialSetHold_ni(con, 0)) {
    cprintf("\nOh bother, ISR configuration failed.  Dying now...");
    return;
  }

  /* At this point, you can't use cprintf anymore because the
   * console's transmit ISR has been co-opted. */

  /* Enable interrupts, which releases the tty to echo */
  __enable_interrupt();

  /* Now blink every nominal half second.  The blink proves that the
   * loop is running even if nothing's coming out the serial port. */
  while (1) {
    BSP430_CORE_DELAY_CYCLES(BSP430_CLOCK_NOMINAL_MCLK_HZ / 2);
    vBSP430ledSet(0, -1);
  }

}
