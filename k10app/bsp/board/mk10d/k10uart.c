/**
 ******************************************************************************
 * @file      k10uart.c
 * @brief     C Source file of k10uart.c.
 * @details   This file including all API functions's 
 *            implement of k10uart.c.	
 *
 * @copyright
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "../../bspcfg.h"
#include <types.h>
#include <stdio.h>
#include <intLib.h>
#include <ttyLib.h>
#include <uart.h>
#include <MK10DZ10.h>

#if (BOARD_BUILD_VER == BOARD_MK10D)

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef struct
{
    uint32_t  base;
    uint16_t  ttyno;
    uint32_t  intid;
} uart_param_t;

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#define MAX_UART    4
#define DEFAULT_BAUDRATE    115200
#define RING_BUF_SIZE       128

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
static void
uart_int(tty_exparam_t *pexparam);

static void
uart_txenable(tty_exparam_t *pexparam, bool_e s);

static void
uart_rxenable(tty_exparam_t *pexparam, bool_e s);

static void
uart_trenable(tty_exparam_t *pexparam, bool_e s);

static int32_t
uart_set_param(tty_exparam_t *pexparam, tty_param_t* p);

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
// UART口的参数表
static const uart_param_t uartParas[] =
{
    {UART0, 0, INT_UART0_RX_TX},
    {UART1, 1, INT_UART1_RX_TX},
    {UART2, 2, INT_UART2_RX_TX},
    {UART3, 3, INT_UART3_RX_TX},
    {UART4, 4, INT_UART4_RX_TX},
    {UART5, 5, INT_UART5_RX_TX}
};

static const tty_opt uartopt =
{
    .tx_enable = uart_txenable,
    .rx_enable = uart_rxenable,
    .tr_enable = uart_trenable,
    .set_param = uart_set_param,
};

static tty_exparam_t the_tty_exparam[MAX_UART];

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
void
tty_init(void)
{
    for (uint8_t i = 0u; i < MAX_UART; i++)
    {
        uart_init(uartParas[i].base, DEFAULT_BAUDRATE);
    }
    for (uint8_t i = 0u; i < ARRAY_SIZE(the_tty_exparam); i++)
    {
        the_tty_exparam[i].baseregs = uartParas[i].base;
        the_tty_exparam[i].popt = &uartopt;
        intConnect(uartParas[i].intid, uart_int, (uint32_t)&the_tty_exparam[i]);
        intEnable(uartParas[i].intid);
    }

    for (uint8_t i = 0u; i < MAX_UART; i++)
    {
        if (OK != tty_create(i, &the_tty_exparam[i], RING_BUF_SIZE, RING_BUF_SIZE))
        {
            printf("tty_create tty%d err\n", i);
        }
    }
    UARTx[UART3]->C2 |= UART_C2_RIE_MASK;
}

/**
 ******************************************************************************
 * @brief   uart中断服务程序
 * @param[in]  *pexparam : uart参数
 *
 * @return  None
 ******************************************************************************
 */
static void
uart_int(tty_exparam_t *pexparam)
{
    uint8_t bufchar;
    uint8_t tmp = UARTx[pexparam->baseregs]->S1;

//    printf("S1[%x]\n", tmp);
    //1. 接收中断
    if ((tmp & (1 << 5)) != 0)
    {
        bufchar = UARTx[pexparam->baseregs]->D;
//        ttylib_putchar(pexparam, bufchar);
        ttylib_putchar(pexparam, '1');
    }

    //2. 发送中断
    if ((tmp & (1 << 7)) != 0)
    {
        if (ttylib_getchar(pexparam, &bufchar) != 0u)
        {
            UARTx[pexparam->baseregs]->D = bufchar;
        }
        else
        {
            /* 关闭发送中断 */
            UARTx[pexparam->baseregs]->C2 &= ~UART_C2_TIE_MASK;
        }
    }
}

/**
 ******************************************************************************
 * @brief   uart设置tx中断
 * @param[in]  *pexparam : uart参数
 * @param[in]  s         : 是否使能中断
 *
 * @return  None
 ******************************************************************************
 */
static void
uart_txenable(tty_exparam_t *pexparam, bool_e s)
{
    if (s == TRUE)
    {
        uint8_t outchar;
        /* 等待发送缓冲区空 */
        while (!(UARTx[pexparam->baseregs]->S1 & UART_S1_TDRE_MASK));
#if 1
        while (!(!(UARTx[pexparam->baseregs]->S1 & UART_S1_TDRE_MASK)))
        {
            if (ttylib_getchar(pexparam, &outchar) != 0u)
            {
                //发送数据
                UARTx[pexparam->baseregs]->D = outchar;
            }
            else
            {
                break;
            }
        }
        UARTx[pexparam->baseregs]->C2 |= UART_C2_TIE_MASK;
#else
        if (ttylib_getchar(pexparam, &outchar) != 0u)
        {
            //发送数据
            UARTx[pexparam->baseregs]->D = outchar;
            UARTx[pexparam->baseregs]->C2 |= UART_C2_TIE_MASK;
        }
#endif
    }
    else
    {
        UARTx[pexparam->baseregs]->C2 &= ~UART_C2_TIE_MASK;
    }
}

/**
 ******************************************************************************
 * @brief   uart设置rx中断
 * @param[in]  *pexparam : uart参数
 * @param[in]  s         : 是否使能中断
 *
 * @return  None
 ******************************************************************************
 */
static void
uart_rxenable(tty_exparam_t *pexparam, bool_e s)
{
    if (s == TRUE)
    {
        UARTx[pexparam->baseregs]->C2 |= UART_C2_RIE_MASK;
    }
    else
    {
        UARTx[pexparam->baseregs]->C2 &= ~UART_C2_RIE_MASK;
    }
}

/**
 ******************************************************************************
 * @brief   是否使能uart
 * @param[in]  *pexparam : uart参数
 * @param[in]  s         : 是否使能
 *
 * @return  None
 ******************************************************************************
 */
static void
uart_trenable(tty_exparam_t *pexparam, bool_e s)
{
    if (s == TRUE)
    {
        UARTx[pexparam->baseregs]->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
        UARTx[pexparam->baseregs]->C1 |= 1 << 7; //使用回环模式
    }
    else
    {
        UARTx[pexparam->baseregs]->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    }
}

/**
 ******************************************************************************
 * @brief   设置uart参数
 * @param[in]  *pexparam : uart参数
 * @param[in]  * p       : 参数
 *
 * @return  None
 ******************************************************************************
 */
static int32_t
uart_set_param(tty_exparam_t *pexparam, tty_param_t* p)
{
#if 0
    p->baudrate;
    p->wordlength;
    p->stopbits;
    p->parity;
#endif
    //todo: 波特率在此设置
    return 0;
}

#endif
/*-------------------------------k10uart.c-----------------------------------*/
