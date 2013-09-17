/**
 ******************************************************************************
 * @file       uart_test.c
 * @version    V1.1.4
 * @brief      API C source file of uart_test.c
 *             Created on: 2013-9-12
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
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
#include <devLib.h>



uint32_t uart_test (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    int32_t _the_tty0_fd, _the_tty1_fd;
    uint8_t i;
    uint8_t s_buff[] = {0x55, 0x55, 0x55, 0x55, 0x55};
    uint8_t r_buff[] = {0x0, 0x0, 0x0, 0x0, 0x0};
    _the_tty0_fd = dev_open("tty0", O_RDWR);
    if(_the_tty0_fd < 0)
    {
        printf("open tty0 error\n");
        return 1;
    }
    _the_tty1_fd = dev_open("tty1", O_RDWR);
    if(_the_tty1_fd < 0)
    {
        printf("open tty1 error\n");
        return 1;
    }
    gpio_init(PORTE, 2, 1, 1);
    gpio_init(PORTA, 16, 1, 1);

    gpio_set(PORTA, 16, 1);
    taskDelay(1);
    dev_write(_the_tty0_fd, s_buff, sizeof(s_buff));
    gpio_set(PORTE, 2, 0);
    taskDelay(1);
    dev_read(_the_tty1_fd, r_buff, sizeof(s_buff));

    for(i = 0; i < 5; i++)
    {
        printf("0x%x ", r_buff[i]);
    }
    printf("\n");
    dev_close(_the_tty0_fd);
    dev_close(_the_tty1_fd);

    return 0;
}
SHELL_CMD(
    uart_t,  CFG_MAXARGS,        uart_test,
    "uart test \r\n"
);
