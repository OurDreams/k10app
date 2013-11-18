/**
 ******************************************************************************
 * @file      uc1698.c
 * @brief     本文实现了uc1698点阵LCD底层驱动.
 * @details   This file including all API functions's implement of uc16981.c.
 *
 * @copyright Copyrigth(C), 2008-2012,Sanxing Electric Co.,Ltd.
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <string.h>
#include <maths.h>
//#include <config.h>
#include <types.h>
#include <gpio.h>
#include <lcm.h>
#include "lcddrv.h"
#include <lcd_wr.h>
#include <taskLib.h>

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#define SCREEN_WIDTH  160
#define SCREEN_HIGHT  160
#define BYTES_OF_LINE 20
#define LCD_SIZE (SCREEN_HIGHT * BYTES_OF_LINE)
#define INCLUDE_LCD_HY 1
//#define INCLUDE_LCD_YS 1

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
static uint8_t layer0[LCD_SIZE];  /* 虚拟显存0层 */

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
extern void LCD_DATA_OUT(uint8_t data);
extern void LCD_CMD_OUT(uint8_t data);
extern char* LCD_GETBASE();

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief   设置lcd背光
 * @param[in]  is_open  : TRUE 打开背光； FALSE 关闭背光
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_set_backlight(bool_e is_open)
{
    if (is_open)
    {
        gpio_set(PORTE, 19, 1);
    }
    else
    {
        gpio_set(PORTE, 19, 0);
    }
}

/**
 ******************************************************************************
 * @brief   清空缓存
 * @param[in]  None
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_clear_layer(void)
{
    memset(layer0, 0x00, LCD_SIZE);
}

/**
 ******************************************************************************
 * @brief   清空行
 * @param[in]  start    : 起始行
 * @param[in]  num      : 行数
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_clear_lines(uint8_t start, uint8_t num)
{
    if (start >= SCREEN_HIGHT)
    {
        return;
    }

    for (uint8_t line = start; line < (num + start); line++)
    {
        memset(layer0 + 20 * line, 0x00, 20);
        if (line >= SCREEN_HIGHT)
        {
            break;
        }
    }

}

/**
 ******************************************************************************
 * @brief   设置点颜色
 * @param[in]  x    : x
 * @param[in]  y    : y
 * @param[in]  color: 颜色
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_set_point(uint8_t x,
        uint8_t y,
        uint8_t color)
{
    if ((x < SCREEN_WIDTH) && (y < SCREEN_HIGHT))
    {
        if (color == 0x00)
        {
            layer0[y * SCREEN_WIDTH / 8u + x / 8u] &= ~(1u << (7u - (x % 8u)));
        }
        else
        {
            layer0[y * SCREEN_WIDTH / 8u + x / 8u] |= (1u << (7u - (x % 8u)));
        }
    }
}

/**
 ******************************************************************************
 * @brief   获取点颜色
 * @param[in]  None
 *
 * @return     None
 ******************************************************************************
 */
static uint8_t
uc1698_get_point(uint8_t x,
        uint8_t y)
{
    uint8_t color = 0xff;

    if ((x < SCREEN_WIDTH) && (y < SCREEN_HIGHT))
    {
        if (layer0[y * SCREEN_WIDTH / 8u + x / 8u] & (1u << (7u - (x % 8u))))
        {
            color = 1u;
        }
        else
        {
            color = 0u;
        }
    }

    return color;
}

#ifdef INCLUDE_LCD_HY
/**
 ******************************************************************************
 * @brief   刷新寄存器(HY)
 * @param[in]  contrast : 对比度(0~10)
 * @param[in]  isNeedCheck: 初始化前是否需要检查状态; true: 需要， false: 不需要
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_refresh_reg_hy(uint8_t contrast, bool_e isNeedCheck)
{
    if (isNeedCheck)
    {
        if (((uint8_t)(*(volatile uint8_t *)LCD_GETBASE()) & 0xB8) == 0xB8)
        {
            return;/* 如果液晶状态寄存器正常，不作操作处理 */
        }
        else
        {
            /* 如果不正常，复位一下液晶，然后再重新更新寄存器 */
            LCD_PowerOn();
        }
    }

    LCD_PowerOn();

    contrast = (contrast > 10u) ? 10u : contrast;

    LCD_CMD_OUT(0xE2); /* System Reset */
    for (volatile int i = 0; i < 100; i++);

    LCD_CMD_OUT(0xAE); /* Set Display Disable */
    for (volatile int i = 0; i < 100; i++);

    LCD_CMD_OUT(0x26); /* Set Temperature Compensation 10-0.15% */
    LCD_CMD_OUT(0x2B); /* Set Power Control (Interal VLCD;Panel loading definition>13nF) */
    LCD_CMD_OUT(0xE9); /* Set LCD Bias ratio:1/10 */

    LCD_CMD_OUT(0x81);  /* Set gain and potentiometer Mode */
    LCD_CMD_OUT(0xb5 - 40 + contrast * 8); /* Program Gain:01;PM value:xx */

    LCD_CMD_OUT(0x89); /* Set RAM Address Controlz */
    LCD_CMD_OUT(0xC4); /* Set LCD Maping Control (MY=1, MX=0) */
    LCD_CMD_OUT(0xDE); /* Set COM Scan Function   for (int i = 0; i < 100; i++) */

    LCD_CMD_OUT(0xC8); /* Set N-Line Inversion */
    LCD_CMD_OUT(0x18); /* Set COM Scan Function */
    LCD_CMD_OUT(0xA3); /* Set Line Rate */
    LCD_CMD_OUT(0xD5); /* Set Color Mode (4K) */
    LCD_CMD_OUT(0xD1); /* Set Color Pattern (RGB) */
    LCD_CMD_OUT(0x84); /* Set Partial Display Off */

    LCD_CMD_OUT(0xf4);
    LCD_CMD_OUT(0x25); /* Start column */
    LCD_CMD_OUT(0xf6);
    LCD_CMD_OUT(0x5a); /* end column */
    LCD_CMD_OUT(0xf5);
    LCD_CMD_OUT(0x00); /* Start row */
    LCD_CMD_OUT(0xf7);
    LCD_CMD_OUT(0x9F); /* end row */

    LCD_CMD_OUT(0xf8);
    LCD_CMD_OUT(0xad);
}

/**
 ******************************************************************************
 * @brief   更新全屏(HY)
 * @param[in]  None
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_updt_all_hy(void)
{
    uint8_t i, j;

    uint8_t tmp;

    LCD_CMD_OUT(0x60); /*row address LSB       */
    LCD_CMD_OUT(0x70); /*row address MSB       */
    LCD_CMD_OUT(0x00 | (37 & 0x0f));        /*column address LSB */
    LCD_CMD_OUT(0x10 | ((37 & 0xf0) >> 4)); /*column address MSB */

    for (i = 0u; i < 160u; i++)
    {
        /* 单行81字节,总共162个像素点 */
        for (j = 0u; j < 160u; j += 2)
        {
            tmp = 0u;
            if (uc1698_get_point(159 - i, j) != 0x00)
            {
                tmp = 0xf0;
            }
            if (uc1698_get_point(159 - i, j + 1) != 0x00)
            {
                tmp |= 0x0f;
            }
            LCD_DATA_OUT(tmp);
        }
        /* 最后补2个像素的1个字节画出最后1个点 */
        tmp = 0;
        LCD_DATA_OUT(tmp);
    }
}

/**
 ******************************************************************************
 * @brief   更新区域(HY)
 * @param[in]  None
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_updt_area_hy(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1)
{
    uint8_t dl;
    uint8_t dr;
    uint8_t tmp;

    x0 = MIN(SCREEN_WIDTH - 1, x0);
    x1 = MIN(SCREEN_WIDTH - 1, x1);
    x0 = MAX(0, x0);
    x1 = MAX(0, x1);

    y0 = MIN(SCREEN_HIGHT - 1, y0);
    y1 = MIN(SCREEN_HIGHT - 1, y1);
    y0 = MAX(0, y0);
    y1 = MAX(0, y1);
#if 1
    tmp = x0;
    x0 = y0;
    y0 = 159 - tmp;

    tmp = x1;
    x1 = y1;
    y1 = 159 - tmp;
#endif
    if (x0 > x1)
    {
        tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1)
    {
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    dl = x0 / 3u;
    dr = x1 / 3u;
    x0 = dl * 3;
    x1 = dr * 3u + 2u;

    for (uint8_t i = y0; i <= y1; i++)
    {
        LCD_CMD_OUT(0x60 | (i & 0x0f));             /*row address LSB       */
        LCD_CMD_OUT(0x70 | ((i & 0xf0) >> 4u));     /*row address MSB       */
        LCD_CMD_OUT(0x00 | ((dl + 0x25) & 0x0f));   /*column address LSB */
        LCD_CMD_OUT(0x10 | (((dl + 0x25) & 0xf0) >> 4u));/*column address MSB */
        for (uint8_t j = x0; j <= x1; j += 2)
        {
            tmp = 0x00;
            if (uc1698_get_point(159 - i, j) != 0x00)
            {
                tmp = 0xf0;
            }
            if (uc1698_get_point(159 - i, j + 1) != 0x00)
            {
                tmp |= 0x0f;
            }
            LCD_DATA_OUT(tmp);
        }
    }
}

/**
 ******************************************************************************
 * @brief   设置对比度(HY)
 * @param[in]  value : 对比度值(0~10)
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_set_contrast_hy(uint8_t value)
{
    value = (value > 10u) ? 10u : value;

    LCD_CMD_OUT(0x81);  /* set VLCD value */
    LCD_CMD_OUT(0xb5 - 40 + value * 8);
}

/**
 ******************************************************************************
 * @brief   lcd初始化方法
 * @param[in]  None
 *
 * @retval  OK    : 初始化成功
 * @retval  ERROR : 初始化失败
 ******************************************************************************
 */
static status_t
uc1698_init_hy(void)
{
    static bool_e first = TRUE;
    if (first)
    {
        first = FALSE;
        uc1698_refresh_reg_hy(5u, FALSE);
        return OK;
    }
    return ERROR;
}

static const lcddrv_t the_uc1698hy_t =
{
    .pname = "HY",
    .width = 160,
    .height = 160,
    .pfn_init = uc1698_init_hy,
    .pfn_updt_all = uc1698_updt_all_hy,
    .pfn_updt_area = uc1698_updt_area_hy,
    .pfn_set_contrast = uc1698_set_contrast_hy,
    .pfn_refresh_reg = uc1698_refresh_reg_hy,
    .pfn_clear_layer = uc1698_clear_layer,
    .pfn_clear_lines = uc1698_clear_lines,
    .pfn_set_backlight = uc1698_set_backlight,
    .pfn_set_point = uc1698_set_point,
    .pfn_get_point = uc1698_get_point,
};

#endif

#ifdef INCLUDE_LCD_YS
/**
 ******************************************************************************
 * @brief   刷新寄存器(YS)
 * @param[in]  contrast : 对比度(0~10)
 * @param[in]  isNeedCheck: 初始化前是否需要检查状态; true: 需要， false: 不需要
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_refresh_reg_ys(uint8_t contrast, bool_e isNeedCheck)
{
#if 0
    if (isNeedCheck)
    {
        if (((uint8_t)(*(volatile uint8_t *)LCD_GETBASE()) & 0xB8) == 0xB8)
        {
            return;/* 如果液晶状态寄存器正常，不作操作处理 */
        }
        else
        {
            /* 如果不正常，复位一下液晶，然后再重新更新寄存器 */
            LCD_PowerOn();
        }
    }
#endif

    contrast = (contrast > 10u) ? 10u : contrast;
    LCD_CMD_OUT(0xe2);
    for (volatile int i = 0; i < 100; i++)
        ;

    /*power control*/
    LCD_CMD_OUT(0x2b); /* set internal power control */
    for (volatile int i = 0; i < 100; i++)
        ;

    LCD_CMD_OUT(0x26); /* set TC=-0.15% */
    LCD_CMD_OUT(0xa3); /* set line rate */
    LCD_CMD_OUT(0xEB); /* set bias:  1/12bias */
    LCD_CMD_OUT(0xf1); /* set com end */
    LCD_CMD_OUT(0x9f); /* duty=1/160 */
    LCD_CMD_OUT(0x81); /* set VLCD value */
    LCD_CMD_OUT(0x2d - 40 + contrast * 8);/* VLCD=(CV0+Cpm*pm)*(1+(T-25)*CT%) */
    LCD_CMD_OUT(0xc2); /* set LCD mapping control  Normal COM, Reverse SEG */
    LCD_CMD_OUT(0xc8); /* INVERSION */
    LCD_CMD_OUT(0x14);
    LCD_CMD_OUT(0x89); /* set ram address control */
    LCD_CMD_OUT(0xd0); /* set color pattern=RGB */
    LCD_CMD_OUT(0xd5); /* set color mode=4k-color */
    LCD_CMD_OUT(0xad); /* set ON/OFF display enable */
    for (volatile int i = 0; i < 100; i++) ;

    LCD_CMD_OUT(0xf8); /* inside mode */
    LCD_CMD_OUT(0xf4);
    LCD_CMD_OUT(0x25); /* Start column */
    LCD_CMD_OUT(0xf5);
    LCD_CMD_OUT(0x00); /* Start row */
    LCD_CMD_OUT(0xf6);
    LCD_CMD_OUT(0x5a); /* end column */
    LCD_CMD_OUT(0xf7);
    LCD_CMD_OUT(0x9F); /* end row */
    LCD_CMD_OUT(0x60);
    LCD_CMD_OUT(0x70);
    LCD_CMD_OUT(0x05);
    LCD_CMD_OUT(0x12);
}

/**
 ******************************************************************************
 * @brief   更新全屏(YS)
 * @param[in]  None
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_updt_all_ys(void)
{
    uint8_t i, j;

    uint8_t tmp;

    LCD_CMD_OUT(0x60); /*row address LSB       */
    LCD_CMD_OUT(0x70); /*row address MSB       */
    LCD_CMD_OUT(0x00 | (0x25 & 0x0f));        /*column address LSB */
    LCD_CMD_OUT(0x10 | ((0x25 & 0xf0) >> 4)); /*column address MSB */

    for (i = 0u; i < 160u; i++)
    {
        /* 单行81字节,总共162个像素点 */
        for (j = 0u; j < 160u; j += 2)
        {
            tmp = 0u;
            if (uc1698_get_point(j, i) != 0x00)
            {
                tmp = 0xf0;
            }
            if (uc1698_get_point(j + 1, i) != 0x00)
            {
                tmp |= 0x0f;
            }
            LCD_DATA_OUT(tmp);
        }
        /* 最后补2个像素的1个字节画出最后1个点 */
        tmp = 0;
        LCD_DATA_OUT(tmp);
    }
}

/**
 ******************************************************************************
 * @brief   更新区域(YS)
 * @param[in]  None
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_updt_area_ys(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1)
{
    uint8_t dl;
    uint8_t dr;
    uint8_t tmp;

    x0 = MIN(SCREEN_WIDTH - 1, x0);
    x1 = MIN(SCREEN_WIDTH - 1, x1);
    x0 = MAX(0, x0);
    x1 = MAX(0, x1);

    y0 = MIN(SCREEN_HIGHT - 1, y0);
    y1 = MIN(SCREEN_HIGHT - 1, y1);
    y0 = MAX(0, y0);
    y1 = MAX(0, y1);

    if (x0 > x1)
    {
        tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1)
    {
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    dl = x0 / 3u;
    dr = x1 / 3u;
    x0 = dl * 3;
    x1 = dr * 3u + 2u;

    for (uint8_t i = y0; i <= y1; i++)
    {
        LCD_CMD_OUT(0x60 | (i & 0x0f));             /*row address LSB       */
        LCD_CMD_OUT(0x70 | ((i & 0xf0) >> 4u));     /*row address MSB       */
        LCD_CMD_OUT(0x00 | ((dl + 0x25) & 0x0f));   /*column address LSB */
        LCD_CMD_OUT(0x10 | (((dl + 0x25) & 0xf0) >> 4u));/*column address MSB */
        for (uint8_t j = x0; j <= x1; j += 2)
        {
            tmp = 0x00;
            if (uc1698_get_point(j, i) != 0x00)
            {
                tmp = 0xf0;
            }
            if (uc1698_get_point(j + 1, i) != 0x00)
            {
                tmp |= 0x0f;
            }
            LCD_DATA_OUT(tmp);
        }
    }
}

/**
 ******************************************************************************
 * @brief   设置对比度(YS)
 * @param[in]  value : 对比度(0~10)
 *
 * @return     None
 ******************************************************************************
 */
static void
uc1698_set_contrast_ys(uint8_t value)
{
    value = (value > 10u) ? 10u : value;

    LCD_CMD_OUT(0x81);  /* set VLCD value */
    LCD_CMD_OUT(0x2d - 40 + value * 8);
}

/**
 ******************************************************************************
 * @brief   lcd初始化方法(YS)
 * @param[in]  None
 *
 * @retval  OK    : 初始化成功
 * @retval  ERROR : 初始化失败
 ******************************************************************************
 */
static status_t
uc1698_init_ys(void)
{
    static bool_e first = TRUE;
    if (first)
    {
        first = FALSE;
        LCD_PowerOn();
        uc1698_refresh_reg_ys(5u, FALSE);  //修改对比度
        return OK;
    }
    return ERROR;
}

static const lcddrv_t the_uc1698ys_t =
{
    .pname = "YS",
    .width = 160,
    .height = 160,
    .pfn_init = uc1698_init_ys,
    .pfn_updt_all = uc1698_updt_all_ys,
    .pfn_updt_area = uc1698_updt_area_ys,
    .pfn_set_contrast = uc1698_set_contrast_ys,
    .pfn_refresh_reg = uc1698_refresh_reg_ys,
    .pfn_clear_layer = uc1698_clear_layer,
    .pfn_clear_lines = uc1698_clear_lines,
    .pfn_set_backlight = uc1698_set_backlight,
    .pfn_set_point = uc1698_set_point,
    .pfn_get_point = uc1698_get_point,
};
#endif

/**
 ******************************************************************************
 * @brief   lcd初始化
 * @param[in]  None
 *
 * @retval     lcd操作方法
 ******************************************************************************
 */

const lcddrv_t const *
uc1698lcd_init(uint8_t type)
{
    (void)type;

#ifdef INCLUDE_LCD_HY
    uc1698_init_hy();
    return &the_uc1698hy_t;
#endif

#ifdef INCLUDE_LCD_YS
    uc1698_init_ys();
    return &the_uc1698ys_t;
#endif

}

/*--------------------------------uc1698.c-----------------------------------*/
