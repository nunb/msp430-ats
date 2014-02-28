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

#ifndef BSP430_PLATFORM_EXP430F5529LP_PLATFORM_H
#define BSP430_PLATFORM_EXP430F5529LP_PLATFORM_H

/** @file
 *
 * @brief Platform-specific BSP430 configuration directives for <a href="http://www.ti.com/tool/msp-exp430f5529lp">MSP-EXP430F5529 USB LaunchPad</a>
 *
 * The following platform-specific features are supported: <ul>
 *
 * <li> #vBSP430platformSpinForJumper_ni The jumper for this platform
 * is P2.0, located at the top right of header J5 on the right of the
 * board below the JTAG header.  Place the jumper between GND and
 * P2.0.
 *
 * <li> #BSP430_PERIPH_EXPOSED_CLOCKS Clocks are made available with
 * ACLK on J8 (connected to red LED); SMCLK on P2.2 at J5.6; MCLK on
 * P4.0 at J5.15.  @warning MCLK is exposed through the port mapping
 * function.  When #BSP430_PERIPH_EXPOSED_CLOCKS is enabled the
 * UCA1CLK function normally assigned to this pin is not available.
 *
 * </ul>
 *
 * @homepage http://github.com/pabigot/bsp430
 * @copyright Copyright 2012-2013, Peter A. Bigot.  Licensed under <a href="http://www.opensource.org/licenses/BSD-3-Clause">BSD-3-Clause</a>
 */

/** Unconditionally define this, so as to produce errors if there is a
 * conflict in definition. */
#define BSP430_PLATFORM_EXP430F5529LP 1

/** EXP430F5529LP has a 4MHz XT2 */
#define BSP430_CLOCK_NOMINAL_XT2CLK_HZ 4000000UL

/** @cond DOXYGEN_EXCLUDE */

/* Available button definitions */
#define BSP430_PLATFORM_BUTTON0_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_PLATFORM_BUTTON0_PORT_BIT BIT1
#define BSP430_PLATFORM_BUTTON1_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT1
#define BSP430_PLATFORM_BUTTON1_PORT_BIT BIT1

/* Standard LED colors */
#define BSP430_LED_RED 0
#define BSP430_LED_GREEN 1

/* Select core voltage: MSP430F5529 8/12/20/25 */
#define BSP430_PMM_COREV_FOR_MCLK(mclk_)    \
  ((20000000UL < (mclk_)) ? PMMCOREV_3 :    \
   ((12000000UL < (mclk_)) ? PMMCOREV_2 :   \
    (((8000000UL < (mclk_)) ? PMMCOREV_1 :  \
      PMMCOREV_0))))

/* How to use ACLK as a capture/compare input source */
/* Settings for TA2: T2A2 ccis=1 ; clk P2.2 ; cc0 P2.3 ; cc1 P2.4 */
#ifndef BSP430_TIMER_CCACLK_ACLK_CCIDX
/* NB: Check against BSP430_TIMER_CCACLK_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_ACLK_CCIDX 2
#endif /* BSP430_TIMER_CCACLK_ACLK_CCIDX */
#ifndef BSP430_TIMER_CCACLK_ACLK_CCIS
/* NB: Check against BSP430_TIMER_CCACLK_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_ACLK_CCIS CCIS_1
#endif /* BSP430_TIMER_CCACLK_ACLK_CCIS */
#ifndef BSP430_TIMER_CCACLK_CLK_PORT_BIT
/* NB: Check against BSP430_TIMER_CCACLK_CLK_PORT_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_CLK_PORT_BIT BIT2
#endif /* BSP430_TIMER_CCACLK_CLK_PORT_BIT */
#ifndef BSP430_TIMER_CCACLK_CC0_PORT_BIT
/* NB: Check against BSP430_TIMER_CCACLK_CC0_PORT_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_CC0_PORT_BIT BIT3
#endif /* BSP430_TIMER_CCACLK_CC0_PORT_BIT */
#ifndef BSP430_TIMER_CCACLK_CC1_PORT_BIT
/* NB: Check against BSP430_TIMER_CCACLK_CC1_PORT_PERIPH_CPPID in bsp430_config.h */
#define BSP430_TIMER_CCACLK_CC1_PORT_BIT BIT4
#endif /* BSP430_TIMER_CCACLK_CC1_PORT_BIT */

/* !BSP430! insert=emk_platform boosterpack=anaren_cc110l platform=exp430f5529lp mcu=msp430f5529 spi=UCB0SOMI tag=cc110x gpio=GDO0,GDO1,GDO2 */
/* BEGIN AUTOMATICALLY GENERATED CODE---DO NOT MODIFY [emk_platform] */
#if (configBSP430_RF_ANAREN_CC110L - 0)
#define BSP430_RF_CC110X 1
#define BSP430_RF_CC110X_SPI_PERIPH_HANDLE BSP430_PERIPH_USCI5_B0
#define BSP430_RF_CC110X_GDO2_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT6
#define BSP430_RF_CC110X_GDO2_PORT_BIT BIT5
#define BSP430_RF_CC110X_GDO0_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RF_CC110X_GDO0_PORT_BIT BIT0
#define BSP430_RF_CC110X_GDO0_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RF_CC110X_GDO0_TIMER_CCIDX 1
#define BSP430_RF_CC110X_GDO0_TIMER_CCIS CCIS_0
#define BSP430_RF_CC110X_CSn_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RF_CC110X_CSn_PORT_BIT BIT2
#define BSP430_RF_CC110X_GDO1_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RF_CC110X_GDO1_PORT_BIT BIT1
#endif /* configBSP430_RF_ANAREN_CC110L */
/* END AUTOMATICALLY GENERATED CODE [emk_platform] */
/* !BSP430! end=emk_platform */

/* !BSP430! insert=rfem_platform boosterpack=ccem platform=exp430f5529lp mcu=msp430f5529 */
/* BEGIN AUTOMATICALLY GENERATED CODE---DO NOT MODIFY [rfem_platform] */
#if (configBSP430_RFEM_CCEM - 0)
#define BSP430_RFEM_CCEM 1
#define BSP430_RFEM 1
#endif /* configBSP430_RFEM_CCEM */
#if (BSP430_RFEM_CCEM - 0)
#define BSP430_RFEM_SERIAL_PERIPH_HANDLE BSP430_PERIPH_USCI5_B0
#define BSP430_RFEM_RF1P3_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P3_PORT_BIT BIT3
#define BSP430_RFEM_RF1P3_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA2
#define BSP430_RFEM_RF1P3_TIMER_CCIDX 0
#define BSP430_RFEM_RF1P3_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF1P7_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P7_PORT_BIT BIT3
#define BSP430_RFEM_RF1P9_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P9_PORT_BIT BIT4
#define BSP430_RFEM_RF1P10_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P10_PORT_BIT BIT0
#define BSP430_RFEM_RF1P10_TIMER_PERIPH_HANDLE BSP430_PERIPH_TA1
#define BSP430_RFEM_RF1P10_TIMER_CCIDX 1
#define BSP430_RFEM_RF1P10_TIMER_CCIS CCIS_0
#define BSP430_RFEM_RF1P12_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT6
#define BSP430_RFEM_RF1P12_PORT_BIT BIT5
#define BSP430_RFEM_RF1P14_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF1P14_PORT_BIT BIT2
#define BSP430_RFEM_RF1P16_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P16_PORT_BIT BIT2
#define BSP430_RFEM_RF1P18_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P18_PORT_BIT BIT0
#define BSP430_RFEM_RF1P20_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT3
#define BSP430_RFEM_RF1P20_PORT_BIT BIT1
#define BSP430_RFEM_RF2P15_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT2
#define BSP430_RFEM_RF2P15_PORT_BIT BIT6
#define BSP430_RFEM_RF2P18_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT8
#define BSP430_RFEM_RF2P18_PORT_BIT BIT1
#define BSP430_RFEM_RF2P19_PORT_PERIPH_HANDLE BSP430_PERIPH_PORT4
#define BSP430_RFEM_RF2P19_PORT_BIT BIT1
#endif /* BSP430_RFEM_CCEM */
/* END AUTOMATICALLY GENERATED CODE [rfem_platform] */
/* !BSP430! end=rfem_platform */

/** @endcond */

/* Include generic file, in case this is being included directly */
#include <bsp430/platform.h>

#endif /* BSP430_PLATFORM_EXP430F5529LP_H */
