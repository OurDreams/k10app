/**
 ******************************************************************************
 * @file      lcm.h
 * @brief     ����ʵ���˺ڰ׵���Һ��GUI������������.
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
    status_t (*pfn_init)(void);     /**< ��ʼ������ */
    void (*pfn_info)(uint8_t *, uint8_t *); /**< ��ȡ���� */
    void (*pfn_clear)(void);        /**< �������� */
    void (*pfn_clear_lines)(uint8_t, uint8_t);  /**< ���� */
    void (*pfn_cross)(uint8_t, uint8_t);/**< ���Ʒ��� */
    void (*pfn_reverse_area)(uint8_t, uint8_t, uint8_t, uint8_t);   /**< ����ѡ */
    void (*pfn_refresh_reg)(uint8_t, bool_e);  /**< ˢ�¼Ĵ��� */
    void (*pfn_set_contrast)(uint8_t);    /**< ���öԱȶ� */
    void (*pfn_set_backlight)(bool_e);  /**< ���ñ��� */
    void (*pfn_updt_area)(uint8_t, uint8_t, uint8_t, uint8_t);  /**< �������� */
    void (*pfn_updt_all)(void);     /**< ����ȫ�� */
    void (*pfn_bmp)(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*); /**< ��bmpͼ */
    void (*pfn_pic)(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*); /**< ��ѹ��ͼ */
    void (*pfn_text)(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t);    /**< д���� */
    void (*pfn_line)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);        /**< ���� */
#if (LCM_LINE_WITH_EN == 1u)
    void (*pfn_line_width)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   /**< ������ */
#endif
    void (*pfn_rectangle)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   /**< ������ */
    void (*pfn_rectangle_fill)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   /**< �������� */
    void (*pfn_square)(uint8_t, uint8_t, uint8_t, uint8_t);         /**< �������� */
    void (*pfn_lines)(uint8_t const *, uint8_t, uint8_t);         /**< ����s */
#if  (LCM_CIRCLE_EN == 1u)
    void (*pfn_circle)(uint8_t, uint8_t, uint8_t, uint8_t);         /**< ��Բ */
    void (*pfn_circle_fill)(uint8_t, uint8_t, uint8_t, uint8_t);    /**< �����Բ */
#endif
#if  (LCM_ELLIPSE_EN == 1u)
    void (*pfn_ellipse)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);         /**< ����Բ */
    void (*pfn_ellipse_fill)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);    /**< �������Բ */
#endif
} lcm_t;

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
extern const lcm_t *
lcm_opt(void);

#endif
/*---------------------------------lcm.h-------------------------------------*/
