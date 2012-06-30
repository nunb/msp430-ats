/** @file
 *
 * Support for detecting the cause of a system reset, and for inducing
 * resets.
 *
 * @author Peter A. Bigot <bigotp@acm.org>
 * @copyright @link http://www.opensource.org/licenses/BSD-3-Clause BSD 3-Clause @endlink
 */

#include <bsp430/5xx/sysrst.h>

/* The list of relevant system reset vector values can be obtained by
 * filtering the MCU-specific headers using this pipe:

 grep -h 'define.*SYSRSTIV' * \
   | sort \
   | grep : \
   | grep -v Reserved \
   | uniq \
   | sort -b -k +3

Values through security violation (SYSRSTIV_SECYV) reflect a BOR;
greater values reflect a lesser reset.

Passing the result through the following filter removes cases where
the constant differs but the symbol and description are the same:

   | sed -e 's@(0x....) *@@' \
   | sort \
   | uniq \

The initializer for the table of expected values can be obtained by
passing that output through:

   | sed -e 's@^[^ ]* \([^ ]*\) .*: \(.*\) \*\/@#ifdef \1%EMIT_SYSRSTIV(\1, "\2")%#endif /\* \1 *\/@' \
   | tr '%\015' '\012\012'

 */

typedef struct sysrstiv_pair_t {
	unsigned char iv;
#if bsp430CONFIG_5XX_SYSRSTIV_WITH_DESCRIPTION - 0
#define EMIT_SYSRSTIV(_iv,_txt) { iv: _iv, desc: _txt },
	const char * desc;
#else
#define EMIT_SYSRSTIV(_iv,_txt) { iv: _iv },
#endif /* bsp430CONFIG_5XX_SYSRSTIV_WITH_DESCRIPTION */
} sysrstiv_pair_t;

static const sysrstiv_pair_t sysrstiv_pairs[] = {

#ifdef SYSRSTIV_BOR
EMIT_SYSRSTIV(SYSRSTIV_BOR, "BOR")
#endif /* SYSRSTIV_BOR */

#ifdef SYSRSTIV_CCSKEY
EMIT_SYSRSTIV(SYSRSTIV_CCSKEY, "CCS Key violation")
#endif /* SYSRSTIV_CCSKEY */

#ifdef SYSRSTIV_CSKEY
EMIT_SYSRSTIV(SYSRSTIV_CSKEY, "CSKEY violation")
#endif /* SYSRSTIV_CSKEY */

#ifdef SYSRSTIV_CSPW
EMIT_SYSRSTIV(SYSRSTIV_CSPW, "CS Password violation")
#endif /* SYSRSTIV_CSPW */

#ifdef SYSRSTIV_DBDIFG
EMIT_SYSRSTIV(SYSRSTIV_DBDIFG, "FRAM Double bit Error")
#endif /* SYSRSTIV_DBDIFG */

#ifdef SYSRSTIV_DOBOR
EMIT_SYSRSTIV(SYSRSTIV_DOBOR, "Do BOR")
#endif /* SYSRSTIV_DOBOR */

#ifdef SYSRSTIV_DOPOR
EMIT_SYSRSTIV(SYSRSTIV_DOPOR, "Do POR")
#endif /* SYSRSTIV_DOPOR */

#ifdef SYSRSTIV_FLLUL
EMIT_SYSRSTIV(SYSRSTIV_FLLUL, "FLL unlock")
#endif /* SYSRSTIV_FLLUL */

#ifdef SYSRSTIV_FRCTLPW
EMIT_SYSRSTIV(SYSRSTIV_FRCTLPW, "FRAM Key violation")
#endif /* SYSRSTIV_FRCTLPW */

#ifdef SYSRSTIV_KEYV
EMIT_SYSRSTIV(SYSRSTIV_KEYV, "Flash Key violation")
#endif /* SYSRSTIV_KEYV */

#ifdef SYSRSTIV_LPM5WU
EMIT_SYSRSTIV(SYSRSTIV_LPM5WU, "Port LPM5 Wake Up")
#endif /* SYSRSTIV_LPM5WU */

#ifdef SYSRSTIV_MPUKEY
EMIT_SYSRSTIV(SYSRSTIV_MPUKEY, "MPUKEY violation")
#endif /* SYSRSTIV_MPUKEY */

#ifdef SYSRSTIV_MPUPW
EMIT_SYSRSTIV(SYSRSTIV_MPUPW, "MPU Password violation")
#endif /* SYSRSTIV_MPUPW */

#ifdef SYSRSTIV_MPUSEG1IFG
EMIT_SYSRSTIV(SYSRSTIV_MPUSEG1IFG, "MPUSEG1IFG violation")
#endif /* SYSRSTIV_MPUSEG1IFG */

#ifdef SYSRSTIV_MPUSEG2IFG
EMIT_SYSRSTIV(SYSRSTIV_MPUSEG2IFG, "MPUSEG2IFG violation")
#endif /* SYSRSTIV_MPUSEG2IFG */

#ifdef SYSRSTIV_MPUSEG3IFG
EMIT_SYSRSTIV(SYSRSTIV_MPUSEG3IFG, "MPUSEG3IFG violation")
#endif /* SYSRSTIV_MPUSEG3IFG */

#ifdef SYSRSTIV_MPUSEGIIFG
EMIT_SYSRSTIV(SYSRSTIV_MPUSEGIIFG, "MPUSEGIIFG violation")
#endif /* SYSRSTIV_MPUSEGIIFG */

#ifdef SYSRSTIV_MPUSEGPIFG
EMIT_SYSRSTIV(SYSRSTIV_MPUSEGPIFG, "MPUSEGPIFG violation")
#endif /* SYSRSTIV_MPUSEGPIFG */

#ifdef SYSRSTIV_PERF
EMIT_SYSRSTIV(SYSRSTIV_PERF, "peripheral/config area fetch")
#endif /* SYSRSTIV_PERF */

#ifdef SYSRSTIV_PLLUL
EMIT_SYSRSTIV(SYSRSTIV_PLLUL, "PLL unlock")
#endif /* SYSRSTIV_PLLUL */

#ifdef SYSRSTIV_PMMKEY
EMIT_SYSRSTIV(SYSRSTIV_PMMKEY, "PMMKEY violation")
#endif /* SYSRSTIV_PMMKEY */

#ifdef SYSRSTIV_PMMPW
EMIT_SYSRSTIV(SYSRSTIV_PMMPW, "PMM Password violation")
#endif /* SYSRSTIV_PMMPW */

#ifdef SYSRSTIV_PSSKEY
EMIT_SYSRSTIV(SYSRSTIV_PSSKEY, "PMMKEY violation")
#endif /* SYSRSTIV_PSSKEY */

#ifdef SYSRSTIV_RSTNMI
EMIT_SYSRSTIV(SYSRSTIV_RSTNMI, "RST/NMI")
#endif /* SYSRSTIV_RSTNMI */

#ifdef SYSRSTIV_SECYV
EMIT_SYSRSTIV(SYSRSTIV_SECYV, "Security violation")
#endif /* SYSRSTIV_SECYV */

#ifdef SYSRSTIV_SVMBOR
EMIT_SYSRSTIV(SYSRSTIV_SVMBOR, "SVMBOR")
#endif /* SYSRSTIV_SVMBOR */

#ifdef SYSRSTIV_SVMH_OVP
EMIT_SYSRSTIV(SYSRSTIV_SVMH_OVP, "SVMH_OVP")
#endif /* SYSRSTIV_SVMH_OVP */

#ifdef SYSRSTIV_SVML_OVP
EMIT_SYSRSTIV(SYSRSTIV_SVML_OVP, "SVML_OVP")
#endif /* SYSRSTIV_SVML_OVP */

#ifdef SYSRSTIV_SVSH
EMIT_SYSRSTIV(SYSRSTIV_SVSH, "SVSH")
#endif /* SYSRSTIV_SVSH */

#ifdef SYSRSTIV_SVSHIFG
EMIT_SYSRSTIV(SYSRSTIV_SVSHIFG, "SVSHIFG")
#endif /* SYSRSTIV_SVSHIFG */

#ifdef SYSRSTIV_SVSL
EMIT_SYSRSTIV(SYSRSTIV_SVSL, "SVSL")
#endif /* SYSRSTIV_SVSL */

#ifdef SYSRSTIV_SVSLIFG
EMIT_SYSRSTIV(SYSRSTIV_SVSLIFG, "SVSLIFG")
#endif /* SYSRSTIV_SVSLIFG */

#ifdef SYSRSTIV_UBDIFG
EMIT_SYSRSTIV(SYSRSTIV_UBDIFG, "FRAM Uncorrectable bit Error")
#endif /* SYSRSTIV_UBDIFG */

#ifdef SYSRSTIV_WDTKEY
EMIT_SYSRSTIV(SYSRSTIV_WDTKEY, "WDTKEY violation")
#endif /* SYSRSTIV_WDTKEY */

#ifdef SYSRSTIV_WDTTO
EMIT_SYSRSTIV(SYSRSTIV_WDTTO, "WDT Time out")
#endif /* SYSRSTIV_WDTTO */

};

portBASE_TYPE
xBSP430sysrstGenerator (unsigned char * pucHaveBOR,
						  const char ** ppcDescription)
{
	uint16_t iv = SYSRSTIV;

	if (SYSRSTIV_NONE == iv) {
		return 0;
	}
	if ((NULL != pucHaveBOR)
		&& (0
#ifdef SYSRSTIV_BOR
			|| (SYSRSTIV_BOR == iv)
#endif /* SYSRSTIV_BOR */
#ifdef SYSRSTIV_RSTNMI
			|| (SYSRSTIV_RSTNMI == iv)
#endif /* SYSRSTIV_RSTNMI */
#ifdef SYSRSTIV_SVMBOR
			|| (SYSRSTIV_SVMBOR == iv)
#endif /* SYSRSTIV_SVMBOR */
#ifdef SYSRSTIV_DOBOR
			|| (SYSRSTIV_DOBOR == iv)
#endif /* SYSRSTIV_DOBOR */
#ifdef SYSRSTIV_LPM5WU
			|| (SYSRSTIV_LPM5WU == iv)
#endif /* SYSRSTIV_LPM5WU */
#ifdef SYSRSTIV_SECYV
			|| (SYSRSTIV_SECYV == iv)
#endif /* SYSRSTIV_SECYV */
			)) {
		*pucHaveBOR = 1;
	}
	if (NULL != ppcDescription) {
#if bsp430CONFIG_5XX_SYSRSTIV_WITH_DESCRIPTION - 0
		const sysrstiv_pair_t * pp = sysrstiv_pairs;
		const sysrstiv_pair_t * epp = pp + sizeof(sysrstiv_pairs)/sizeof(*sysrstiv_pairs);

		while ((pp < epp) && (pp->iv != iv)) {
			++pp;
		}
		*ppcDescription = (pp < epp) ? pp->desc : "?";
#else /* bsp430CONFIG_5XX_SYSRSTIV_WITH_DESCRIPTION */
		*ppcDescription = "?";
#endif /* bsp430CONFIG_5XX_SYSRSTIV_WITH_DESCRIPTION */
	}
	return iv;
}
