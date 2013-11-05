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
    LT_YS = 0,     /**< ��˳ */
    LT_HY,         /**< ��Զ */
} lcd_type_e;

typedef struct
{
    const char_t * pname;               /**< lcd�� */
    uint8_t width;                        /**< �� */
    uint8_t height;                       /**< �� */
    status_t (*pfn_init)(void);         /**< ��ʼ������ */
    void (*pfn_updt_all)(void);         /**< ����ȫ������ */
    void (*pfn_updt_area)(uint8_t, uint8_t, uint8_t, uint8_t);  /**< �������򷽷� */
    void (*pfn_set_contrast)(uint8_t);    /**< ���öԱȶȷ��� */
    void (*pfn_refresh_reg)(uint8_t, bool_e);      /**< ˢ�¼Ĵ������� */
    void (*pfn_set_backlight)(bool_e);  /**< ���ñ��� */
    void (*pfn_clear_layer)(void);      /**< ������淽�� */
    void (*pfn_clear_lines)(uint8_t, uint8_t);  /**< ����� */
    void (*pfn_set_point)(uint8_t, uint8_t, uint8_t); /**< ���㷽�� */
    uint8_t (*pfn_get_point)(uint8_t, uint8_t);       /**< ��ȡ����ɫ���� */
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
