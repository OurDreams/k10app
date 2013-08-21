/*
 * --------------"拉普兰德K60底层库"-----------------
 *
 * 测试硬件平台:LPLD_K60 Card
 * 版权所有:北京拉普兰德电子技术有限公司
 * 网络销售:http://laplenden.taobao.com
 * 公司门户:http://www.lpld.cn
 *
 * 文件名: HAL_PIT.h
 * 用途: PIT底层模块相关函数
 * 最后修改日期: 20120321
 *
 * 开发者使用协议:
 *  本代码面向所有使用者开放源代码，开发者可以随意修改源代码。但本段及以上注释应
 *  予以保留，不得更改或删除原版权所有者姓名。二次开发者可以加注二次版权所有者，
 *  但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 */
#ifndef __PIT_H__
#define __PIT_H__
/********************************************************************/

typedef void (*PIT_ISR_CALLBACK)(void);

typedef enum
{
    PIT0 = 0,
    PIT1,
    PIT2,
    PIT3,
}PITx;

uint8_t LPLD_PIT_Init(PITx, uint32_t, PIT_ISR_CALLBACK);
uint8_t LPLD_PIT_SetIsr(PITx, PIT_ISR_CALLBACK);
uint8_t LPLD_PIT_ClearIsr(PITx);
void LPLD_PIT_Isr(void);
void clear_pitflag(PITx pitx);

#endif /* __HAL_PIT_H__ */
