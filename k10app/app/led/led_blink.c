/**
 ******************************************************************************
 * @file      led.c
 * @brief     C Source file of led.c.
 * @details   This file including all API functions's 
 *            implement of led.c.	
 *
 * @copyright
 ******************************************************************************
 */
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <devLib.h>
#include <gpio.h>
#include <taskLib.h>
#include <osLib.h>
#include <dmnLib.h>
#include <pit.h>
#include <led.h>
/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
 #define CS0_START_ADDRESS ((uint8_t*)(0xA0000000))
 #define CS1_START_ADDRESS ((uint8_t*)(0xA1000000))
 #define CS2_START_ADDRESS ((uint8_t*)(0xA2000000))
 #define CS4_START_ADDRESS ((uint8_t*)(0xA4000000))
 #define CS5_START_ADDRESS ((uint8_t*)(0xA5000000))

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
void pit_test(void)
{
    clear_pitflag(0);
    printf("test\n");
}
/**
 ******************************************************************************
 * @brief   流水灯任务
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static void ledTask(void *p_arg)
{
//    uint8_t led = 0xff;
	DMN_ID dmnid = dmn_register();
	int32_t led_fd;
	led_fd = dev_open("leds", O_RDWR);

	FOREVER
	{
		dmn_sign(dmnid);
		dev_ioctl(led_fd, led_turn, 0);
#if 0
		*CS0_START_ADDRESS = led;
        *CS1_START_ADDRESS = led;
        *CS2_START_ADDRESS = led;
        *CS4_START_ADDRESS = led;
        *CS5_START_ADDRESS = led--;
		taskDelay(osClkRateGet()/4);
#endif
		taskDelay(osClkRateGet()/4);
	}
}

void led_task_init(void)
{
    taskSpawn((const signed char*)"led", 3, 256, (OSFUNCPTR)ledTask,0);
}

