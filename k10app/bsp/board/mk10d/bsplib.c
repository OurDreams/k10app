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
uint32_t core_clk_khz;       //�ں�ʱ��(KHz)
uint32_t core_clk_mhz;       //�ں�ʱ��(MHz)
uint32_t bus_clk_khz;        //��Χ����ʱ��

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
//-------------------------------------------------------------------------*
//������: trace_clk_init                                                   *
//��  ��: ����ʱ�ӳ�ʼ��                                                   *
//��  ��: ��                                  *
//��  ��: ��                                                               *
//˵  ��: ���ڵ���                                                         *
//-------------------------------------------------------------------------*
void trace_clk_init(void)
{
    /* Set the trace clock to the core clock frequency */
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    /* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
    PORTA_PCR6 = ( PORT_PCR_MUX(0x7));
}
/********************************************************************/

//-------------------------------------------------------------------------*
//������: fb_clk_init                                                      *
//��  ��: FlexBusʱ�ӳ�ʼ��                                                *
//��  ��: ��                                  *
//��  ��: ��                                                               *
//˵  ��:                                                                  *
//-------------------------------------------------------------------------*
void fb_clk_init(void)
{
    /* Enable the clock to the FlexBus module */
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

    /* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}

void
mcuClkSetup(void)
{
    /* ʹ�����ж˿�PORTʱ��   */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );

#if defined(NO_PLL_INIT)
    core_clk_mhz = 20;                      //������岻��ʼ�����໷������� FEI mode
#else
    /* ��ǿϵͳƵ��   �������������ó�Ƶ  */
    core_clk_mhz = pll_init(MCG_CLK_MHZ);
#endif

    //ͨ��pll_init�����ķ���ֵ�������ں�ʱ�Ӻ�����ʱ�ӣ��������������ɲ�ѯʱ��Ƶ��
    core_clk_khz = core_clk_mhz * 1000;
    bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);

    trace_clk_init();                       //ʹ�ܸ���ʱ�ӣ����ڵ���
    fb_clk_init();                          //FlexBusʱ�ӳ�ʼ��

    uart_init(FIRE_PORT, FIRE_BAUD);    //��ʼ�� printf �������õ��Ĵ���
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


/* 5. ��ȡMCU��Ƶ */
uint32_t
bsp_get_mcu_clk(void)
{
    return CPU_CLOCK_HZ;
}

/* 6. ��ȡMCU�ж����� */
uint32_t
bsp_get_max_int_count(void)
{
    return MAX_INT_COUNT;
}

#endif
/*----------------------------bsplib.c--------------------------------*/
