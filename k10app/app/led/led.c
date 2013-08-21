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
/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* NONE */
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
    uint8_t led = 0u;
	int32_t fd = -1;
	DMN_ID dmnid = dmn_register();

	fd = dev_open("gpio", 0);
	if (0 >= fd)
	{
		printf("open gpio error\n");
//		return ;
	}
	if(1 != LPLD_PIT_Init(0, 1000000, pit_test))
	{
	    printf("pit init error\n");
	}
	else
	{
	    printf("pit init ok\n");
	}
	FOREVER
	{
		dmn_sign(dmnid);
		dev_ioctl(fd, (led >> 2) & 0x01, (void *)(led & 0x03));
		led++;
		taskDelay(osClkRateGet() / 4);
	}
	dev_close(fd);
	dev_release("gpio");
}

void led_task_init(void)
{
    taskSpawn((const signed char*)"led", 3, 512, (OSFUNCPTR)ledTask,0);
}

