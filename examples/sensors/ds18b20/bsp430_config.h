/* Use a crystal if one is installed.  Much more accurate timing
 * results. */
#define BSP430_PLATFORM_BOOT_CONFIGURE_LFXT1 1

/* Application does output: support spin-for-jumper */
#ifndef configBSP430_PLATFORM_SPIN_FOR_JUMPER
#define configBSP430_PLATFORM_SPIN_FOR_JUMPER 1
#endif /* configBSP430_PLATFORM_SPIN_FOR_JUMPER */

#define configBSP430_CONSOLE 1
#define configBSP430_UPTIME 1
#define configBSP430_UPTIME_DELAY 1

#if (BSP430_PLATFORM_SURF - 0)
/* SuRF has a DS1825 on P3.7, but the software is the same.  We use
 * the platform-provided resource.  No parasitic power support. */
#define configBSP430_PLATFORM_SURF_DS1825 1
#define APP_DS18B20_BUS xBSP430surfDS1825
#elif (BSP430_PLATFORM_EXP430F5529 - 0)
#define APP_DS18B20_PORT_HAL BSP430_HAL_PORT7
#define APP_DS18B20_BIT BIT7
#ifndef APP_DS18B20_POWER_BIT
#define APP_DS18B20_POWER_BIT BIT1
#endif /* APP_DS18B20_POWER_BIT */
#define configBSP430_HAL_PORT7 1
#else /* BSP430_PLATFORM */
/* External hookup DS18B20 to on P1.6  */
#define APP_DS18B20_PORT_HAL BSP430_HAL_PORT1
#define APP_DS18B20_BIT BIT6
#ifndef APP_DS18B20_POWER_BIT
#define APP_DS18B20_POWER_BIT BIT4
#endif /* APP_DS18B20_POWER_BIT */
#define configBSP430_HAL_PORT1 1
#endif /* BSP430_PLATFORM_SURF */

/* Get platform defaults */
#include <bsp430/platform/bsp430_config.h>
