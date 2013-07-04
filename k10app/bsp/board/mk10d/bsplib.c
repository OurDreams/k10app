/**
 ******************************************************************************
 * @file      bsplib.c
 * @brief     C Source file of bsplib.c.
 * @details   This file including all API functions's 
 *            implement of bsplib.c.	
 *
 * @copyright
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "../../bspcfg.h"
#include <types.h>
#include <intLib.h>
#include <gpio.h>
#include <MK10DZ10.h>
#include <k60_fire.h>
#include <uart.h>

#if (BOARD_BUILD_VER == BOARD_MK10D)


/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
#define UART_BASE             ((USART_TypeDef *) USART6_BASE)

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
uint32_t core_clk_khz;       //内核时钟(KHz)
uint32_t core_clk_mhz;       //内核时钟(MHz)
uint32_t bus_clk_khz;        //外围总线时钟

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
void
mcuClkSetup(void)
{
    /* 使能所有端口PORT时钟   */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );

#if defined(NO_PLL_INIT)
    core_clk_mhz = 20;                      //如果定义不初始化锁相环，则进入 FEI mode
#else
    /* 增强系统频率   可以在这里设置超频  */
    core_clk_mhz = pll_init(MCG_CLK_MHZ);
#endif

    //通过pll_init函数的返回值来计算内核时钟和外设时钟，便于其他函数可查询时钟频率
    core_clk_khz = core_clk_mhz * 1000;
    bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);

    trace_clk_init();                       //使能跟踪时钟，用于调试
    fb_clk_init();                          //FlexBus时钟初始化

    uart_init(FIRE_PORT, FIRE_BAUD);    //初始化 printf 函数所用到的串口
}

void
bspHwInit(void)
{
    intLock();

    intUnlock();
}

int32_t bsp_getchar(void)
{
    int32_t c;

    return c;
}

void bsp_putchar(char_t ch)
{

}

void bsp_reboot(void)
{

}

void bsp_dev_init(void)
{

}

void
bsp_timer_start(void)
{

}

uint32_t
bsp_timer_get(void)
{
    uint32_t starttime_ms = 0;

    return starttime_ms;
}


/* 5. 获取MCU主频 */
uint32_t
bsp_get_mcu_clk(void)
{
    return CPU_CLOCK_HZ;
}

/* 6. 获取MCU中断数量 */
uint32_t
bsp_get_max_int_count(void)
{
    return MAX_INT_COUNT;
}

#endif
/*----------------------------bsplib.c--------------------------------*/
