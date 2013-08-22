/**
 ******************************************************************************
 * @file       flexbus_test.c
 * @version    V1.1.4
 * @brief      API C source file of flexbus_test.c
 *             Created on: 2013-7-25
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C), 2008-2012,Sanxing Electric Co.,Ltd.
 *
 ******************************************************************************
 */

#include <shell.h>
#include <flexbus.h>
#include <pit.h>
#include <stdio.h>
#include <taskLib.h>
#include <uart.h>
#include <k60_fire.h>
#include <gpio.h>
extern uint32_t us;
uint32_t flx_write (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    while(1)
    {
        *CS0_START_ADDRESS = 0x00;
        *CS1_START_ADDRESS = 0x00;
        *CS2_START_ADDRESS = 0x00;
        *CS4_START_ADDRESS = 0x00;
        *CS5_START_ADDRESS = 0x00;
        taskDelay(100);
        *CS0_START_ADDRESS = 0xff;
        *CS1_START_ADDRESS = 0xff;
        *CS2_START_ADDRESS = 0xff;
        *CS4_START_ADDRESS = 0xff;
        *CS5_START_ADDRESS = 0xff;
        taskDelay(100);

        /*ÈÎÒâ×Ö·ûÍË³ö*/
        if(uart_query(FIRE_PORT))
        {
            return 1;
        }

    }
    return 0;
}
SHELL_CMD(
    flx_w,  CFG_MAXARGS,        flx_write,
    "flexbus write\r\n"
);

uint32_t flx_close (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    *CS0_START_ADDRESS = 0xff;
    return 0;
}
SHELL_CMD(
    flx_c,  CFG_MAXARGS,        flx_close,
    "flexbus close\r\n"
);
#if 1
uint32_t flx_wr (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    uint8_t temp = 0;
    uint32_t t1, t2;
    *CS0_START_ADDRESS = 0x78;
    t1 = us;
    while(temp != 0x78)
    {
        temp = *CS0_START_ADDRESS;
        taskDelay(1);
#if 0
        while(us % 10 != 0)
        {
            printf("%d\n", us);
            printf("read %d \n", temp);
        }
#endif
    }
    t2 = us;
    printf("time is %d \r\n", t2 - t1);
    return 0;
}

SHELL_CMD(
    flx_w_r,  CFG_MAXARGS,        flx_wr,
    "flexbus write then read\r\n"
);
#endif

