/* Copyright 2012-2014, Peter A. Bigot
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

#include <bsp430/platform.h>
#include <bsp430/periph/bc2.h>
#if (BSP430_BC2_TRIM_TO_MCLK - 0)
#include <bsp430/periph/timer.h>
#endif /* BSP430_BC2_TRIM_TO_MCLK */

/** The last frequency configured using #ulBSP430clockConfigureMCLK_ni */
static unsigned long configuredMCLK_Hz = BSP430_CLOCK_PUC_MCLK_HZ;

#define SELS_MASK (SELS)
#define SELM_MASK (SELM0 | SELM1)
#define SELA_MASK (LFXT1S0 | LFXT1S1)
#define DIVM_MASK (DIVM0 | DIVM1)
#define DIVS_MASK (DIVS0 | DIVS1)
#define DIVA_MASK (DIVA0 | DIVA1)

int
iBSP430clockConfigureLFXT1_ni (int enablep,
                               int loop_limit)
{
  int loop_delta;
  int rc = 0;

  BSP430_CLOCK_CLEAR_FAULTS_NI();
  if (enablep && (0 != loop_limit)) {
    rc = iBSP430platformConfigurePeripheralPins_ni(BSP430_PERIPH_LFXT1, 0, 1);
    if (0 == rc) {
      loop_delta = (0 < loop_limit) ? 1 : 0;

      /* See whether the crystal is populated and functional.  Do
       * this with the DCO reset to the power-up configuration,
       * where clock should be nominal 1 MHz.
       *
       * @TODO: Preserve XT2 configuration */
      BCSCTL3 = BSP430_CLOCK_LFXT1_XCAP;
      do {
        loop_limit -= loop_delta;
        BSP430_CORE_WATCHDOG_CLEAR();
        BSP430_CORE_DELAY_CYCLES(BSP430_CLOCK_LFXT1_STABILIZATION_DELAY_CYCLES);
      } while ((BSP430_BC2_LFXT1_IS_FAULTED_NI()) && (0 != loop_limit));
      rc = ! BSP430_BC2_LFXT1_IS_FAULTED_NI();
    }
  }
  BSP430_CLOCK_OSC_CLEAR_FAULT_NI();
  if (! rc) {
    (void)iBSP430platformConfigurePeripheralPins_ni(BSP430_PERIPH_LFXT1, 0, 0);
    /* Explicitly fall back to VLOCLK and disable capacitors */
    BCSCTL3 = LFXT1S_2;
  }
  return rc;
}

eBSP430clockSource xBSP430clockACLKSource ()
{
  switch (BCSCTL3 & SELA_MASK) {
    case LFXT1S_0:
      return eBSP430clockSRC_XT1CLK;
    case LFXT1S_1:
      return eBSP430clockSRC_UNKNOWN_1;
    case LFXT1S_2:
      return eBSP430clockSRC_VLOCLK;
    case LFXT1S_3:
      return eBSP430clockSRC_XT2CLK;
  }
  return eBSP430clockSRC_NONE;
}

eBSP430clockSource xBSP430clockSMCLKSource ()
{
  if (BCSCTL2 & SELS) {
    return eBSP430clockSRC_DCOCLK;
  }
  return eBSP430clockSRC_XT2CLK;
}

eBSP430clockSource xBSP430clockMCLKSource ()
{
  switch (BCSCTL2 & SELM_MASK) {
    case SELM_0:
      return eBSP430clockSRC_DCOCLK;
    case SELM_1:
      return eBSP430clockSRC_DCOCLK;
    case SELM_2:
      return eBSP430clockSRC_XT2CLK;
    case SELM_3:
      return eBSP430clockSRC_XT1CLK;
  }
  return eBSP430clockSRC_NONE;
}

int
iBSP430clockConfigureACLK_ni (eBSP430clockSource sel,
                              unsigned int dividing_shift)
{
  unsigned int sela = 0;
  switch (sel) {
    default:
    case eBSP430clockSRC_NONE:
      return -1;
    case eBSP430clockSRC_XT1CLK:
      sela = LFXT1S_0;
      break;
    case eBSP430clockSRC_VLOCLK:
      sela = LFXT1S_2;
      break;
    case eBSP430clockSRC_REFOCLK:
    case eBSP430clockSRC_DCOCLK:
    case eBSP430clockSRC_DCOCLKDIV:
    case eBSP430clockSRC_XT2CLK:
      return -1;
    case eBSP430clockSRC_XT1CLK_FALLBACK:
    case eBSP430clockSRC_XT1CLK_OR_VLOCLK:
      sela = BSP430_CLOCK_LFXT1_IS_FAULTED_NI() ? LFXT1S_2 : LFXT1S_0;
      break;
    case eBSP430clockSRC_XT1CLK_OR_REFOCLK:
      return -1;
  }
  BCSCTL1 = (BCSCTL1 & ~DIVA_MASK) | (DIVA_MASK & (dividing_shift * DIVA0));
  BCSCTL3 = (BCSCTL3 & ~SELA_MASK) | sela;
  return 0;
}

int
iBSP430clockConfigureSMCLK_ni (eBSP430clockSource sel,
                               unsigned int dividing_shift)
{
  unsigned int sels = 0;

#if (configBSP430_PERIPH_XT2 - 0)
  if (eBSP430clockSRC_XT2CLK == sel) {
    sels = SELS;
  }
#endif /* configBSP430_PERIPH_XT2 */
  BCSCTL2 = (BCSCTL2 & ~(SELS | DIVS_MASK)) | sels | (DIVS_MASK & (dividing_shift * DIVS0));
  return 0;
}

unsigned long
ulBSP430clockMCLK_Hz_ni (void)
{
  int div = (BCSCTL2 & DIVM_3) / DIVM0;
  return configuredMCLK_Hz >> div;
}

unsigned long
ulBSP430clockSMCLK_Hz_ni (void)
{
  int div = (BCSCTL2 & DIVS_3) / DIVS0;
  return configuredMCLK_Hz >> div;
}

unsigned long
ulBSP430clockACLK_Hz_ni (void)
{
  unsigned long clk_hz;
  int div = (BCSCTL1 & DIVA_3) / DIVA0;
  switch (BCSCTL3 & SELA_MASK) {
    default:
    case LFXT1S_0:
      if (BSP430_CLOCK_LFXT1_IS_FAULTED_NI()) {
        clk_hz = BSP430_CLOCK_NOMINAL_VLOCLK_HZ;
      } else {
        clk_hz = BSP430_CLOCK_NOMINAL_XT1CLK_HZ;
      }
      break;
    case LFXT1S_2:
      clk_hz = BSP430_CLOCK_NOMINAL_VLOCLK_HZ;
      break;
  }
  return clk_hz >> div;
}

#if (BSP430_BC2_TRIM_TO_MCLK - 0)

int
iBSP430bc2TrimToMCLK_ni (unsigned long mclk_Hz)
{
  volatile sBSP430hplTIMER * tp = xBSP430hplLookupTIMER(BSP430_TIMER_CCACLK_PERIPH_HANDLE);
  const int MAX_ITERATIONS = 16 * 256;
  int rv = -1;
  unsigned long aclk_Hz;
  int iter = 0;
  const int SAMPLE_PERIOD_ACLK = 10;
  unsigned char bcsctl3;
  unsigned int target_tsp;

  if (! tp) {
    return -1;
  }

  bcsctl3 = BCSCTL3;
  if (0 != iBSP430clockConfigureACLK_ni(eBSP430clockSRC_XT1CLK_OR_VLOCLK)) {
    return -1;
  }
  aclk_Hz = ulBSP430clockACLK_Hz_ni();
  target_tsp = (SAMPLE_PERIOD_ACLK * mclk_Hz) / aclk_Hz;
  tp->ctl = TASSEL_2 | MC_2 | TACLR;
  /* SELM = DCOCLK; DIVM = /1 */
  BCSCTL2 &= ~(SELM_MASK | DIVM_MASK);
  while (iter++ < MAX_ITERATIONS) {
    unsigned int freq_tsp;
    freq_tsp = uiBSP430timerCaptureDelta_ni(BSP430_TIMER_CCACLK_PERIPH_HANDLE,
                                            BSP430_TIMER_CCACLK_ACLK_CCIDX,
                                            CM_2,
                                            BSP430_TIMER_CCACLK_ACLK_CCIS,
                                            SAMPLE_PERIOD_ACLK);
    if (freq_tsp == target_tsp) {
      configuredMCLK_Hz = mclk_Hz;
      rv = 0;
      break;
    }
    if (target_tsp < freq_tsp) {
      /* DCO too fast.  Decrement modulator; if underflowed,
       * decrement RSEL */
      if (0xFF == --DCOCTL) {
        --BCSCTL1;
      }
    } else {
      /* DCO too slow.  Increment modulator; if overflowed,
       * increment RSEL */
      if (0 == ++DCOCTL) {
        ++BCSCTL1;
      }
    }
  }
  tp->ctl = 0;
  BCSCTL3 = bcsctl3;
  return rv;
}
#endif /* BSP430_BC2_TRIM_TO_MCLK */

unsigned long
ulBSP430clockConfigureMCLK_ni (unsigned long mclk_Hz)
{
  unsigned char dcoctl;
  unsigned char bcsctl1;
  unsigned long error_Hz;
  int use_trim_to_mclk = 1;
  long freq_Hz;

  if (0 == mclk_Hz) {
    mclk_Hz = BSP430_CLOCK_PUC_MCLK_HZ;
    use_trim_to_mclk = 0;
  }
  /* Power-up defaults */
  dcoctl = 0x60;
  bcsctl1 = 0x87;
  freq_Hz = BSP430_CLOCK_PUC_MCLK_HZ;

  /* Calculate absolute error from _freq_Hz to target */
#define ERROR_HZ(freq_Hz_) ((mclk_Hz < freq_Hz_) ? (freq_Hz_ - mclk_Hz) : (mclk_Hz - freq_Hz_))
  error_Hz = ERROR_HZ(freq_Hz);
  (void)error_Hz;

  /* Test a candidate to see if it's better than what we've got now */
#define TRY_FREQ(tag_, cand_Hz_) do {                   \
    unsigned long cand_error_Hz = ERROR_HZ(cand_Hz_);   \
    if (cand_error_Hz < error_Hz) {                     \
      dcoctl = CALDCO_##tag_;                           \
      bcsctl1 = CALBC1_##tag_;                          \
      freq_Hz = cand_Hz_;                               \
      error_Hz = cand_error_Hz;                         \
    }                                                   \
  } while (0)

  /* Candidate availability is MCU-specific and can be determined by
   * checking for a corresponding preprocessor definition */
#if defined(CALDCO_1MHZ_)
  TRY_FREQ(1MHZ, 1000000UL);
#else
#error No calibration constants
#endif /* CALDCO_1MHZ */
#if defined(CALDCO_8MHZ_)
  TRY_FREQ(8MHZ, 8000000UL);
#endif /* CALDCO_8MHZ */
#if defined(CALDCO_12MHZ_)
  TRY_FREQ(12MHZ, 12000000UL);
#endif /* CALDCO_12MHZ */
#if defined(CALDCO_16MHZ_)
  TRY_FREQ(16MHZ, 16000000UL);
#endif /* CALDCO_16MHZ */

#undef TRY_FREQ
#undef ERROR_HZ

  /* Clearing DCO bits first supports workaround for erratum BCL12 */
  DCOCTL = 0;
  BCSCTL1 = bcsctl1;
  DCOCTL = dcoctl;
  /* SELM = DCOCLK; DIVM = /1 */
  BCSCTL2 &= ~(SELM_MASK | DIVM_MASK);
  configuredMCLK_Hz = freq_Hz;

  if (use_trim_to_mclk) {
#if (BSP430_BC2_TRIM_TO_MCLK - 0)
    (void)iBSP430bc2TrimToMCLK_ni(mclk_Hz);
#endif /* BSP430_BC2_TRIM_TO_MCLK */
  }

  /* Spin until DCO faults cleared */
  do {
    BSP430_CLOCK_CLEAR_FAULTS_NI();
    BSP430_CORE_WATCHDOG_CLEAR();
    /* Conservatively assume a 32 MHz clock */
    BSP430_CORE_DELAY_CYCLES(32 * BSP430_CLOCK_FAULT_RECHECK_DELAY_US);
  } while (BSP430_CLOCK_OSC_IS_FAULTED_NI());

  return configuredMCLK_Hz;
}
