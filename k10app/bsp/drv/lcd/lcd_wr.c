/**
 ******************************************************************************
 * @file       lcd_wr.c
 * @version    V1.1.4
 * @brief      API C source file of lcd_wr.c
 *             Created on: 2013-11-2
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
 ******************************************************************************
 */
#include <stddef.h>
#include <gpio.h>
#include <flexbus.h>
#include <stdio.h>
#include <taskLib.h>

/**
 ******************************************************************************
 * @brief       drv_lcd_getbase: get lcd base reg addr
 * @param[in]   None
 * @return      None
 *
 * @note        ´ýÐÞ¸Ä
 ******************************************************************************
 */
char* LCD_GETBASE()
{
    return NULL;
}
/**
 ******************************************************************************
 * @brief       drv_cmd_out: uc1698 Ð´ÃüÁîº¯Êý
 * @param[in]   None
 * @return      None
 *
 * @note        A0  0: CMD  1: DATA
 ********************************************************
110219016(ÁºÔö»Ô) 13:38:21
**********************
 */
void LCD_CMD_OUT(uint8_t cmd)
{
    gpio_set(PORTD, 6, 0);
    *CS0_START_ADDRESS = cmd;
}

/**
 ******************************************************************************
 * @brief       drv_data_out: uc1698 Ð´Êý¾Ýº¯Êý
 * @param[in]   None
 * @return      None
 *
 * @note        A0  0: CMD  1: DATA
 ******************************************************************************
 */
void LCD_DATA_OUT(uint8_t data)
{
    gpio_set(PORTD, 6, 1);
    *CS0_START_ADDRESS = data;
    //printf("%02x ", data);
}

void LCD_PowerOn(void)
{
    gpio_init(PORTE, 19, 1, 1);
    gpio_init(PORTD, 6, 1, 1);
    gpio_init(PORTE, 26, 1, 1);


    gpio_set(PORTE, 26, 0);
    taskDelay(10);
    gpio_set(PORTE, 26, 1);
    taskDelay(15);

//    printf("lcd power on\n");
}
