/*
 * @file     hw_flexbus.c
 * @brief    GPIO的底层固件库
 * @version  V0.0.1
 * @date     2013.5.27
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013.5.27      梁增辉       第一版
 *
 */
 #ifndef _HW_FLEXBUS_H
 #define _HW_FLEXBUS_H
 /*-----------------------------------------------------------------------------
 Section: Include
------------------------------------------------------------------------------*/
#include "MK10DZ10.h"
 /*-----------------------------------------------------------------------------
 Section: Macro Defines
------------------------------------------------------------------------------*/
 #define CS0_START_ADDRESS ((uint8_t*)(0xA0000000))
 #define CS1_START_ADDRESS ((uint8_t*)(0xA1000000))
 #define CS2_START_ADDRESS ((uint8_t*)(0xA2000000))
 #define CS4_START_ADDRESS ((uint8_t*)(0xA4000000))
 #define CS5_START_ADDRESS ((uint8_t*)(0xA5000000))

 void hw_flexbus_init(void);

 #endif
 /*-------------------------------- hw_flexbus.h --------------------------------*/
