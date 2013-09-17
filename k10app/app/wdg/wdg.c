/**
 ******************************************************************************
 * @file       wdg.c
 * @version    V1.1.4
 * @brief      API C source file of wdg.c
 *             Created on: 2013-9-17
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <devLib.h>
#include <gpio.h>
#include <taskLib.h>
#include <osLib.h>
#include <dmnLib.h>

/**
 ******************************************************************************
 * @brief   流水灯任务
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static void wdgTask(void *p_arg)
{
        DMN_ID dmnid = dmn_register();
        int32_t wdg_fd = dev_open("wdg", O_RDWR);

        FOREVER
        {
            dmn_sign(dmnid);
            dev_ioctl(wdg_fd, 1, 0);
//            taskDelay(osClkRateGet()/4);
            taskDelay(100);
        }
}

void wdg_task_init(void)
{
    taskSpawn((const signed char*)"wdg", 3, 256, (OSFUNCPTR)wdgTask,0);
}
