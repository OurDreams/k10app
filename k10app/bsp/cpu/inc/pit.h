/*
 * --------------"��������K60�ײ��"-----------------
 *
 * ����Ӳ��ƽ̨:LPLD_K60 Card
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * ��������:http://laplenden.taobao.com
 * ��˾�Ż�:http://www.lpld.cn
 *
 * �ļ���: HAL_PIT.h
 * ��;: PIT�ײ�ģ����غ���
 * ����޸�����: 20120321
 *
 * ������ʹ��Э��:
 *  ��������������ʹ���߿���Դ���룬�����߿��������޸�Դ���롣�����μ�����ע��Ӧ
 *  ���Ա��������ø��Ļ�ɾ��ԭ��Ȩ���������������ο����߿��Լ�ע���ΰ�Ȩ�����ߣ�
 *  ��Ӧ�����ش�Э��Ļ����ϣ�����Դ���롢���ó��۴��뱾��
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
