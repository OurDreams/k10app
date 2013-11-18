/**
 ******************************************************************************
 * @file       yx_app.c
 * @version    V1.1.4
 * @brief      API C source file of yx_app.c
 *             Created on: 2013-11-7
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
#include <pit.h>
#include <led.h>
#include <cfg.h>
//#include <yx_drv.h>

/**
 ******************************************************************************
 * @brief   流水灯任务
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static void yxTask(void *p_arg)
{
    DMN_ID dmnid = dmn_register();
    int32_t yx_fd;
    uint8_t buf[5] = {0};
    uint8_t i, j ,k;
    yx_fd = dev_open("yx", O_RDWR);

    dev_read(yx_fd, buf, 0);
    if(buf[4] != 0xff)
    {
        k = 4;//32路
    }
    else
    {
        k = 5;//40路
    }

    FOREVER
    {
        dmn_sign(dmnid);
        dev_read(yx_fd, buf, 0);
        for(i = 0; i < k; i ++)
        {
            for(j = 0; j < 8; j ++)
            {
                if((buf[i] >> j & 0x01) == 0x00)
                {
                    printf("遥信 cs%d %d\n", i, j);
                }
            }
        }
        taskDelay(10);

    }
}

void yx_task_init(void)
{
    taskSpawn((const signed char*)"yx", 5, 1024, (OSFUNCPTR)yxTask, 0);
}
