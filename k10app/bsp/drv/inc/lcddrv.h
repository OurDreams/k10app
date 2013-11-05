/**
 ******************************************************************************
 * @file       lcddrv.h
 * @brief      API include file of lcddrv.h.
 * @details    This file including all API functions's declare of lcddrv.h.
 * @copyright  Copyrigth(C), 2008-2012,Sanxing Electric Co.,Ltd.
 *
 ******************************************************************************
 */
#ifndef __LCDDRV_H__
#define __LCDDRV_H__

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <types.h>

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    LT_YS = 0,     /**< 宇顺 */
    LT_HY,         /**< 华远 */
} lcd_type_e;

typedef struct
{
    const char_t * pname;               /**< lcd名 */
    uint8_t width;                        /**< 宽 */
    uint8_t height;                       /**< 高 */
    status_t (*pfn_init)(void);         /**< 初始化方法 */
    void (*pfn_updt_all)(void);         /**< 更新全屏方法 */
    void (*pfn_updt_area)(uint8_t, uint8_t, uint8_t, uint8_t);  /**< 更新区域方法 */
    void (*pfn_set_contrast)(uint8_t);    /**< 设置对比度方法 */
    void (*pfn_refresh_reg)(uint8_t, bool_e);      /**< 刷新寄存器方法 */
    void (*pfn_set_backlight)(bool_e);  /**< 设置背光 */
    void (*pfn_clear_layer)(void);      /**< 清除缓存方法 */
    void (*pfn_clear_lines)(uint8_t, uint8_t);  /**< 清除行 */
    void (*pfn_set_point)(uint8_t, uint8_t, uint8_t); /**< 画点方法 */
    uint8_t (*pfn_get_point)(uint8_t, uint8_t);       /**< 获取点颜色方法 */
} lcddrv_t;

/*-----------------------------------------------------------------------------
 Section: Globals
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
const lcddrv_t *
lcddrv_init(void);


#endif /* __LCDDRV_H__ */
/*-----------------------------End of lcddrv.h-------------------------------*/
