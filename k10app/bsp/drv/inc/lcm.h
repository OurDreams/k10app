/**
 ******************************************************************************
 * @file      lcm.h
 * @brief     本文实现了黑白点阵液晶GUI操作基础方法.
 * @details   This file including all API functions's implement of lcm.h.
 *
 * @copyright Copyrigth(C), 2008-2012,Sanxing Electric Co.,Ltd.
 ******************************************************************************
 */
#ifndef __LCM_H__
#define __LCM_H__

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <types.h>

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define LCM_LINE_WITH_EN    (1u)
#define LCM_CIRCLE_EN       (1u)
#define LCM_ELLIPSE_EN      (1u)

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef struct
{
    status_t (*pfn_init)(void);     /**< 初始化方法 */
    void (*pfn_info)(uint8_t *, uint8_t *); /**< 获取长宽 */
    void (*pfn_clear)(void);        /**< 清屏方法 */
    void (*pfn_clear_lines)(uint8_t, uint8_t);  /**< 清行 */
    void (*pfn_cross)(uint8_t, uint8_t);/**< 绘制方格 */
    void (*pfn_reverse_area)(uint8_t, uint8_t, uint8_t, uint8_t);   /**< 区域反选 */
    void (*pfn_refresh_reg)(uint8_t, bool_e);  /**< 刷新寄存器 */
    void (*pfn_set_contrast)(uint8_t);    /**< 设置对比度 */
    void (*pfn_set_backlight)(bool_e);  /**< 设置背光 */
    void (*pfn_updt_area)(uint8_t, uint8_t, uint8_t, uint8_t);  /**< 更新区域 */
    void (*pfn_updt_all)(void);     /**< 更新全屏 */
    void (*pfn_bmp)(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*); /**< 画bmp图 */
    void (*pfn_pic)(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*); /**< 画压缩图 */
    void (*pfn_text)(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t);    /**< 写文字 */
    void (*pfn_line)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);        /**< 画线 */
#if (LCM_LINE_WITH_EN == 1u)
    void (*pfn_line_width)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   /**< 画宽线 */
#endif
    void (*pfn_rectangle)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   /**< 画矩形 */
    void (*pfn_rectangle_fill)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   /**< 画填充矩形 */
    void (*pfn_square)(uint8_t, uint8_t, uint8_t, uint8_t);         /**< 画正方形 */
    void (*pfn_lines)(uint8_t const *, uint8_t, uint8_t);         /**< 画线s */
#if  (LCM_CIRCLE_EN == 1u)
    void (*pfn_circle)(uint8_t, uint8_t, uint8_t, uint8_t);         /**< 画圆 */
    void (*pfn_circle_fill)(uint8_t, uint8_t, uint8_t, uint8_t);    /**< 画填充圆 */
#endif
#if  (LCM_ELLIPSE_EN == 1u)
    void (*pfn_ellipse)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);         /**< 画椭圆 */
    void (*pfn_ellipse_fill)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);    /**< 画填充椭圆 */
#endif
} lcm_t;

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
extern const lcm_t *
lcm_opt(void);

#endif
/*---------------------------------lcm.h-------------------------------------*/
