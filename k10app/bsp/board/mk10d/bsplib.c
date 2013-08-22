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
#include <mcg.h>
#include <flash.h>
#include <flexbus.h>
#include <wdog.h>
#include <led.h>

#if (BOARD_BUILD_VER == BOARD_MK10D)


/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
#define UART_BASE             ((USART_TypeDef *) USART6_BASE)

enum clk_option
{
  PLL50,
  PLL100,
  PLL96,
  PLL48,
  PLL72
};

enum crystal_val
{
  XTAL2,
  XTAL4,
  XTAL6,
  XTAL8,
  XTAL10,
  XTAL12,
  XTAL14,
  XTAL16,
  XTAL18,
  XTAL20,
  XTAL22,
  XTAL24,
  XTAL26,
  XTAL28,
  XTAL30,
  XTAL32,
  CLK50 = 24 // special case for external 50 MHz canned osc
};
/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
  /*
   * System Bus Clock Info
   */
  #define MGG_OSC_MODE        XTAL_MODE     // EXTOSC_MODE or XTAL_MODE
  #define REF_CLK             XTAL12         // see available frequencies in osc_frequency   in mcg.h
  #define CORE_CLK_MHZ        PLL72         // see pll_options  frequencies in              in mcg.h


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
//    wdog_disable();
    /* 使能所有端口PORT时钟   */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );

#if defined(NO_PLL_INIT)
       core_clk_mhz = 21; //FEI mode
#else
    /* Ramp up the system clock */
    core_clk_mhz = pll_init(CORE_CLK_MHZ, REF_CLK);
#endif


    //通过pll_init函数的返回值来计算内核时钟和外设时钟，便于其他函数可查询时钟频率
    core_clk_khz = core_clk_mhz * 1000;
    bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);

    uart_init(FIRE_PORT, FIRE_BAUD);    //初始化 printf 函数所用到的串口
}

void
bspHwInit(void)
{
    intLock();

    intUnlock();
}

/**
 ******************************************************************************
 * @brief   0. 默认关闭看门狗
 * @param[in]  None
 *
 * @retrun    None
 ******************************************************************************
 */
void
bsp_close_watchdog(void)
{
    wdog_disable();
}

int32_t bsp_getchar(void)
{
    int32_t c = 0;
    if (uart_query(FIRE_PORT) != 0)
    {
        c = uart_getchar(FIRE_PORT);
    }

    return c;
}

void bsp_putchar(char_t ch)
{
    uart_putchar(FIRE_PORT, ch);
}


void bsp_dev_init(void)
{
//    extern void start_info(void);
//    start_info();
    LPLD_Flash_Init();
    hw_flexbus_init();
//    led_device_init();
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
