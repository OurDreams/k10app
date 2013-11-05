/**
 ******************************************************************************
 * @file       lcd_show.c
 * @version    V1.1.4
 * @brief      API C source file of lcd_show.c
 *             Created on: 2013-11-2
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
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
#include <lcddrv.h>
#include <lcm.h>

/**
 ******************************************************************************
 * @brief   流水灯任务
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static void lcdTask(void *p_arg)
{
    const lcm_t* lcd_opt;
//    DMN_ID dmnid = dmn_register();
    gpio_init(PORTE, 19, 1, 1);
    gpio_init(PORTD, 6, 1, 1);
//    gpio_set(PORTE, 19, 1);
    lcm_opt()->pfn_set_backlight(TRUE);
    lcd_opt = lcm_opt();
    lcd_opt->pfn_init();
//    lcd_opt->pfn_circle_fill(50, 50, 100, 100);
    lcm_opt()->pfn_cross(0,255);
    lcm_opt()->pfn_updt_all();
    while (1)
    {
        taskDelay(1);
    }
}

void lcd_task_init(void)
{
    taskSpawn((const signed char*)"lcd", 4, 2048, (OSFUNCPTR)lcdTask,0);
}
