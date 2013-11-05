/**
 ******************************************************************************
 * @file      usrapp.c
 * @brief     本文实现应用程序的初始化.
 * @details   This file including all API functions's implement of usrapp.c.
 *
 * @copyright
 ******************************************************************************
 */

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <types.h>
#include <led.h>
#include <pit.h>
#include <cfg.h>
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
extern void led_task_init(void);
extern void lcd_task_init(void);
/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
uint32_t us = 0;
void pit0_timer(void)
{
    us ++;
    clear_pitflag(0);
}

/**
 ******************************************************************************
 * @brief   应用程序初始化
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
void
usrapp_init(void)
{
    led_task_init();
    wdg_task_init();
#ifdef DTU_LCD
    lcd_task_init();
#endif
    return;
}

/*--------------------------------usrapp.c-----------------------------------*/
