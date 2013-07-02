/* Copyright 2012-2013, Peter A. Bigot
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the software nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BSP430_PLATFORM_EXP430F5529_PLATFORM_H
#define BSP430_PLATFORM_EXP430F5529_PLATFORM_H

/** @file
 *
 * @brief Platform-specific include for <a href="http://www.ti.com/tool/msp-exp430f5529">MSP-EXP430F5529</a>
 *
 * The following platform-specific features are supported: <ul>
 *
 * <li> #vBSP430platformSpinForJumper_ni The jumper for this platform
 * is P7.7, located at the bottom of header J5 on the right of the
 * board below the JTAG header.  Place the jumper between GND and
 * P7.7.
 *
 * <li> #BSP430_PERIPH_EXPOSED_CLOCKS ACLK is made visible on P1.0
 * which can be found on the J12 or RF1 header.  MCLK is made visible
 * on P7.7 which is on header J5 below the JTAG header.  SMCLK is made
 * visible on P2.2 which is not brought out to any accessible
 * location.
 *
 * </ul>
 *
 * @homepage http://github.com/pabigot/bsp430
 * @copyright Copyright 2012-2013, Peter A. Bigot.  Licensed under <a href="http://www.opensource.org/licenses/BSD-3-Clause">BSD-3-Clause</a>
 */

/** Unconditionally define this, so as to produce errors if there is a
 * conflict in definition. */
#define BSP430_PLATFORM_EXP430F5529 1

/** EXP430F5529 has a 4MHz XT2 */
#define BSP430_CLOCK_NOMINAL_XT2CLK_HZ 4000000UL

/** Define to control CCACLK capabilities for EXP430F5529 platform
 *
 * The EXP430F5529 board does a very poor job of making signals
 * accessible.  No timer has all of CLK, CC0, and CC1 on header pins.
 *
 * If this is defined to a true value, selection of
 * #configBSP430_TIMER_CCACLK will select a timer for which the CLK
 * signal can be accessed from a board header.  If defined to a false
 * value (default), #configBSP430_TIMER_CCACLK will select a timer for
 * which the CC0 and CC1 signals can be accessed from a board
 * header. */
#ifndef configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK
#define configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK 0
#endif /* configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK */

/** @cond DOXYGEN_EXCLUDE */

/* Enable if requested (ez430 serial needs it) */
#if (configBSP430_PLATFORM_SPIN_FOR_JUMPER - 0)
#define BSP430_PLATFORM_SPIN_FOR_JUMPER 1
#endif /* configBSP430_PLATFORM_SPIN_FOR_JUMPER */

/* Available button definitions */
#define BSP430_PLATFORM_BUTTON0_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT1
#define BSP430_PLATFORM_BUTTON0_PORT_BIT BIT7
#define BSP430_PLATFORM_BUTTON1_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_PLATFORM_BUTTON1_PORT_BIT BIT2

/* Standard LED colors */
#define BSP430_LED_RED 0
#define BSP430_LED_ORANGE 1
#define BSP430_LED_GREEN 2
#define BSP430_LED_BLUE 3
#define BSP430_LED_BLUE1 4
#define BSP430_LED_BLUE2 5
#define BSP430_LED_BLUE3 6
#define BSP430_LED_BLUE4 7

/* Select core voltage: MSP430F5529 8/12/20/25 */
#define BSP430_PMM_COREV_FOR_MCLK(_mclk)    \
  ((20000000UL < (_mclk)) ? PMMCOREV_3 :    \
   ((12000000UL < (_mclk)) ? PMMCOREV_2 :   \
    (((8000000UL < (_mclk)) ? PMMCOREV_1 :  \
      PMMCOREV_0))))

/* How to use ACLK as a capture/compare input source.  This board does
 * a very poor job of making signals accessible.  No timer has all of
 * CLK, CC0, and CC1 on header pins.
 *
 * With CLK: Settings for TB0: T0B6 ccis=1 ; clk P7.7 ; cc0 P5.6 ; cc1 P5.7 -- CC0/CC1 PINS NOT ACCESSIBLE
 *
 * Without CLK: Settings for TA2: T2A2 ccis=1 ; clk P2.2 ; cc0 P2.3 ; cc1 P2.4 -- CLK PIN NOT ACCESSIBLE
 */

#ifndef BSP430_TIMER_CCACLK_ACLK_CCIDX
/* NB: Check against BSP430_TIMER_CCACLK_PERIPH_CPPID in bsp430_config.h */
#if (configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK - 0)
#define BSP430_TIMER_CCACLK_ACLK_CCIDX 6
#else /* configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK */
#define BSP430_TIMER_CCACLK_ACLK_CCIDX 2
#endif /* configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK */
#endif /* BSP430_TIMER_CCACLK_ACLK_CCIDX */
#ifndef BSP430_TIMER_CCACLK_ACLK_CCIS
/* NB: Check against BSP430_TIMER_CCACLK_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_ACLK_CCIS CCIS_1
#endif /* BSP430_TIMER_CCACLK_ACLK_CCIS */

#if (configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK - 0)
/* Option preferring CLK */

#ifndef BSP430_TIMER_CCACLK_CLK_PORT_BIT
/* NB: Check against BSP430_TIMER_CCACLK_CLK_PORT_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_CLK_PORT_BIT BIT7
#endif /* BSP430_TIMER_CCACLK_CLK_PORT_BIT */

#else /* configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK */
/* Option preferring CC0/CC1 */
#ifndef BSP430_TIMER_CCACLK_CC0_PORT_BIT
/* NB: Check against BSP430_TIMER_CCACLK_CC0_PORT_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_CC0_PORT_BIT BIT3
#endif /* BSP430_TIMER_CCACLK_CC0_PORT_BIT */
#ifndef BSP430_TIMER_CCACLK_CC1_PORT_BIT
/* NB: Check against BSP430_TIMER_CCACLK_CC1_PORT_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_CC1_PORT_BIT BIT4
#endif /* BSP430_TIMER_CCACLK_CC1_PORT_BIT */

#endif /* configBSP430_PLATFORM_EXP430F5529_CCACLK_NEED_CLK */

/* !BSP430! insert=rfem_platform platform=exp430f5529 mcu=msp430f5529 */
/* BEGIN AUTOMATICALLY GENERATED CODE---DO NOT MODIFY [rfem_platform] */
#if (configBSP430_RFEM - 0)
#define BSP430_RFEM 1
#endif /* configBSP430_RFEM */
#if (BSP430_RFEM - 0)
#define BSP430_RFEM_SERIAL_PERIPH_HANDLE BSP430_PERIPH_USCI5_B0
#define BSP430_RFEM_RF1P3_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P3_PORT_BIT BIT1
#define BSP430_RFEM_RF1P3_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RFEM_RF1P3_TIMER_CCIDX 2
#define BSP430_RFEM_RF1P3_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF1P5_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P5_PORT_BIT BIT0
#define BSP430_RFEM_RF1P5_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RFEM_RF1P5_TIMER_CCIDX 1
#define BSP430_RFEM_RF1P5_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF1P6_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF1P6_PORT_BIT BIT0
#define BSP430_RFEM_RF1P7_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF1P7_PORT_BIT BIT0
#define BSP430_RFEM_RF1P8_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF1P8_PORT_BIT BIT6
#define BSP430_RFEM_RF1P9_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF1P9_PORT_BIT BIT6
#define BSP430_RFEM_RF1P10_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P10_PORT_BIT BIT3
#define BSP430_RFEM_RF1P10_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA2
#define BSP430_RFEM_RF1P10_TIMER_CCIDX 0
#define BSP430_RFEM_RF1P10_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF1P12_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P12_PORT_BIT BIT4
#define BSP430_RFEM_RF1P12_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA2
#define BSP430_RFEM_RF1P12_TIMER_CCIDX 1
#define BSP430_RFEM_RF1P12_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF1P14_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P14_PORT_BIT BIT7
#define BSP430_RFEM_RF1P16_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P16_PORT_BIT BIT2
#define BSP430_RFEM_RF1P18_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P18_PORT_BIT BIT0
#define BSP430_RFEM_RF1P20_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P20_PORT_BIT BIT1
#define BSP430_RFEM_RF2P13_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT1
#define BSP430_RFEM_RF2P13_PORT_BIT BIT0
#define BSP430_RFEM_RF2P15_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF2P15_PORT_BIT BIT0
#define BSP430_RFEM_RF2P15_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RFEM_RF2P15_TIMER_CCIDX 1
#define BSP430_RFEM_RF2P15_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF2P18_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF2P18_PORT_BIT BIT7
#define BSP430_RFEM_RF2P19_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT7
#define BSP430_RFEM_RF2P19_PORT_BIT BIT5
#define BSP430_RFEM_RF2P19_TIMER_PERIPH_HANDLE BSP430_PERIPH_TB0
#define BSP430_RFEM_RF2P19_TIMER_CCIDX 3
#define BSP430_RFEM_RF2P19_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF2P20_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT6
#define BSP430_RFEM_RF2P20_PORT_BIT BIT6
#define BSP430_RFEM_RF3P3_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT1
#define BSP430_RFEM_RF3P3_PORT_BIT BIT0
#define BSP430_RFEM_RF3P4_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF3P4_PORT_BIT BIT0
#define BSP430_RFEM_RF3P4_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RFEM_RF3P4_TIMER_CCIDX 1
#define BSP430_RFEM_RF3P4_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF3P5_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF3P5_PORT_BIT BIT4
#define BSP430_RFEM_RF3P6_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF3P6_PORT_BIT BIT0
#define BSP430_RFEM_RF3P7_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF3P7_PORT_BIT BIT5
#define BSP430_RFEM_RF3P8_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF3P8_PORT_BIT BIT6
#define BSP430_RFEM_RF3P9_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF3P9_PORT_BIT BIT7
#define BSP430_RFEM_RF3P10_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF3P10_PORT_BIT BIT4
#define BSP430_RFEM_RF3P10_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA2
#define BSP430_RFEM_RF3P10_TIMER_CCIDX 1
#define BSP430_RFEM_RF3P10_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF3P11_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT7
#define BSP430_RFEM_RF3P11_PORT_BIT BIT5
#define BSP430_RFEM_RF3P11_TIMER_PERIPH_HANDLE BSP430_PERIPH_TB0
#define BSP430_RFEM_RF3P11_TIMER_CCIDX 3
#define BSP430_RFEM_RF3P11_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF3P13_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF3P13_PORT_BIT BIT3
#define BSP430_RFEM_RF3P13_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA2
#define BSP430_RFEM_RF3P13_TIMER_CCIDX 0
#define BSP430_RFEM_RF3P13_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF3P14_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF3P14_PORT_BIT BIT1
#define BSP430_RFEM_RF3P14_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RFEM_RF3P14_TIMER_CCIDX 2
#define BSP430_RFEM_RF3P14_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF3P15_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF3P15_PORT_BIT BIT2
#define BSP430_RFEM_RF3P16_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF3P16_PORT_BIT BIT2
#define BSP430_RFEM_RF3P17_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF3P17_PORT_BIT BIT6
#define BSP430_RFEM_RF3P18_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF3P18_PORT_BIT BIT0
#endif /* BSP430_RFEM */
/* END AUTOMATICALLY GENERATED CODE [rfem_platform] */
/* !BSP430! end=rfem_platform */

/* Enable U8GLIB if requested */
#define BSP430_UTILITY_U8GLIB (configBSP430_UTILITY_U8GLIB - 0)

/** @endcond */

#ifndef configBSP430_PLATFORM_EXP430F5529_LCD
/** Enable HPL support for on-board LCD
 *
 * The EXP430F5529 has a DOGS102-6 monochromatic 102x64 pixel LCD on
 * board, including back-light.  Power is hard-wired, with backlight
 * PWM on TB0.4 and control through pins on ports 5 and 7.
 *
 * Defining this constant to a true value enables the configuration of
 * the HPL for ports 5 and 7 and the SPI interface.
 *
 * @see #configBSP430_UTILITY_U8GLIB
 * @defaulted
 * @cppflag */
#define configBSP430_PLATFORM_EXP430F5529_LCD 0
#endif /* configBSP430_PLATFORM_EXP430F5529_LCD */

#if defined(BSP430_DOXYGEN) || (configBSP430_PLATFORM_EXP430F5529_LCD - 0)

/** Peripheral handle for SPI access to LCD */
#define BSP430_PLATFORM_EXP430F5529_LCD_SPI_PERIPH_HANDLE BSP430_PERIPH_USCI5_B1

/** BSP430 peripheral handle for port to which LCD reset (inverted) is connected.
 * @dependency #configBSP430_PLATFORM_EXP430F5529_LCD
 */
#define BSP430_PLATFORM_EXP430F5529_LCD_RSTn_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT5

/** Port bit on #BSP430_PLATFORM_EXP430F5529_LCD_RSTn_PORT_PERIPH_HANDLE for LCD RSTn
 * @dependency #configBSP430_PLATFORM_EXP430F5529_LCD
 */
#define BSP430_PLATFORM_EXP430F5529_LCD_RSTn_PORT_BIT BIT7

/** BSP430 peripheral handle for port to which LCD chip-select (inverted) is connected.
 * @dependency #configBSP430_PLATFORM_EXP430F5529_LCD
 */
#define BSP430_PLATFORM_EXP430F5529_LCD_CSn_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT7

/** Port bit on #BSP430_PLATFORM_EXP430F5529_LCD_CSn_PORT_PERIPH_HANDLE for LCD CSn
 * @dependency #configBSP430_PLATFORM_EXP430F5529_LCD
 */
#define BSP430_PLATFORM_EXP430F5529_LCD_CSn_PORT_BIT BIT4

/** BSP430 peripheral handle for port to which LCD A0 (CMD=0, DATA=1) is connected.
 * @dependency #configBSP430_PLATFORM_EXP430F5529_LCD
 */
#define BSP430_PLATFORM_EXP430F5529_LCD_A0_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT5

/** Port bit on #BSP430_PLATFORM_EXP430F5529_LCD_A0_PORT_PERIPH_HANDLE for LCD A0
 * @dependency #configBSP430_PLATFORM_EXP430F5529_LCD
 */
#define BSP430_PLATFORM_EXP430F5529_LCD_A0_PORT_BIT BIT6

/** Width, in pixel columns, of the DOGS102-6 display */
#define BSP430_PLATFORM_EXP430F5529_LCD_COLUMNS 102

/** Height, in pages, of the DOGS102-6 display */
#define BSP430_PLATFORM_EXP430F5529_LCD_PAGES 8

/** Height, in pixel rows, of each page of the DOGS102-6 display */
#define BSP430_PLATFORM_EXP430F5529_LCD_ROWS_PER_PAGE 8

#endif /* configBSP430_PLATFORM_EXP430F5529_LCD */

/* Include generic file, in case this is being included directly */
#include <bsp430/platform.h>

#endif /* BSP430_PLATFORM_EXP430F5529_H */
