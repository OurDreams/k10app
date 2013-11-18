/**
 ******************************************************************************
 * @file       yk_app.c
 * @version    V1.1.4
 * @brief      API C source file of yk_app.c
 *             Created on: 2013-11-15
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
#include <relay.h>

/**
 ******************************************************************************
 * @brief   Ò£¿ØÌøÕ¢ÈÎÎñ
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static void ykTask(void *p_arg)
{
//    DMN_ID dmnid = dmn_register();
    int32_t yk_fd;
    uint8_t i;
    relay_param_t p;
    yk_fd = dev_open("relays", O_RDWR);
#if 0
    FOREVER
    {
        for(i = 10; i <20; i++)
        {
            p.num = i;
            p.onoff = 0;
            dev_ioctl(yk_fd , 0, (void*)&p);
            taskDelay(20);
        }
        taskDelay(2000);
        for(i = 10; i <20; i++)
        {
            p.num = i;
            p.onoff = 1;
            dev_ioctl(yk_fd , 0, (void*)&p);
            taskDelay(1);
        }
        taskDelay(6000);
//        dmn_sign(dmnid);

    }
#endif

    FOREVER
    {
        for(i = 0; i <20; i++)
        {
            p.num = i;
            p.onoff = 0;
            dev_ioctl(yk_fd , 0, (void*)&p);
            taskDelay(40);

            p.onoff = 1;
            dev_ioctl(yk_fd , 0, (void*)&p);
            taskDelay(40);
        }
    }

    dev_close(yk_fd);

}

void yk_task_init(void)
{
    taskSpawn((const signed char*)"yk", 5, 1024, (OSFUNCPTR)ykTask, 0);
}
