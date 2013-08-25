/*
 * --------------"��������K60�ײ��"-----------------
 *
 * ����Ӳ��ƽ̨:LPLD_K60 Card
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * ��������:http://laplenden.taobao.com
 * ��˾�Ż�:http://www.lpld.cn
 *
 * �ļ���: HAL_PIT.c
 * ��;: PIT�ײ�ģ����غ���
 * ����޸�����: 20120321
 *
 * ������ʹ��Э��:
 *  ��������������ʹ���߿���Դ���룬�����߿��������޸�Դ���롣�����μ�����ע��Ӧ
 *  ���Ա��������ø��Ļ�ɾ��ԭ��Ȩ���������������ο����߿��Լ�ע���ΰ�Ȩ�����ߣ�
 *  ��Ӧ�����ش�Э��Ļ����ϣ�����Դ���롢���ó��۴��뱾��
 */

/*
 *******���õ���ʱ�жϣ�����isr.h��ճ��һ�´���:*********

//PITģ���жϷ�����
#undef  VECTOR_084
#define VECTOR_084 LPLD_PIT_Isr
#undef  VECTOR_085
#define VECTOR_085 LPLD_PIT_Isr
#undef  VECTOR_086
#define VECTOR_086 LPLD_PIT_Isr
#undef  VECTOR_087
#define VECTOR_087 LPLD_PIT_Isr
//���º�����LPLD_Kinetis�ײ���������޸�
extern void LPLD_PIT_Isr(void);

 ***********************�������*************************
*/
#include <MK10DZ10.h>
#include <pit.h>
#include <types.h>
#include <stddef.h>
#include <intLib.h>
#include <stdio.h>

//��������ʱ��Ƶ�ʣ���ֵ��sysinit.c�ļ���ȡ��
extern uint32_t core_clk_mhz;

//�û��Զ����жϷ���������
PIT_ISR_CALLBACK PIT_ISR[4];

/*
 * LPLD_PIT_Init
 * PITͨ�ó�ʼ������
 * 
 * ����:
 *    pitx--PITģ���
 *      |__PIT0        --�����ж϶�ʱ��0
 *      |__PIT1        --�����ж϶�ʱ��0
 *      |__PIT2        --�����ж϶�ʱ��0
 *      |__PIT3        --�����ж϶�ʱ��0
 *    period_us--�ж�����
 *      |__��λΪ΢��
 *    isr_func--�û��жϳ�����ڵ�ַ
 *      |__�û��ڹ����ļ��¶�����жϺ���������������Ϊ:�޷���ֵ,�޲���(eg. void isr(void);)
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */
uint8_t LPLD_PIT_Init(PITx pitx, uint32_t period_us, PIT_ISR_CALLBACK isr_func)
{ 
  uint32_t ldval;
//  uint32_t core_clk_mhz;
  
  
  //���㶨ʱ����ֵ
//  core_clk_mhz = periph_clk_khz/1000;
  ldval = period_us*core_clk_mhz;

  
  //������ʱģ��ʱ��
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
  
  // ���� PIT
  PIT_MCR = 0x00;
#if 0
  if(isr_func!=NULL){
    PIT_ISR[pitx] = isr_func;
#endif
    intConnect(84+pitx, isr_func, 0);
    //��NVIC��ʹ��PIT�ж�
    intEnable(84+pitx);
 // }
  
  //period = (period_ns/bus_period_ns)-1
  PIT_LDVAL_REG(PIT_BASE_PTR,pitx) = ldval-1;
  
  //ʹ���ж�
  PIT_TCTRL_REG(PIT_BASE_PTR,pitx) = PIT_TCTRL_TIE_MASK;
  //��ʼ��ʱ
  PIT_TCTRL_REG(PIT_BASE_PTR,pitx) |= PIT_TCTRL_TEN_MASK;
  
  return 1;
}

/*
 * LPLD_PIT_SetIsr
 * ����PITͨ���û�������жϷ���������ʹ���ж�
 * 
 * ����:
 *    pitx--PITģ���
 *      |__PIT0        --�����ж϶�ʱ��0
 *      |__PIT1        --�����ж϶�ʱ��0
 *      |__PIT2        --�����ж϶�ʱ��0
 *      |__PIT3        --�����ж϶�ʱ��0
 *    isr_func--�û��жϳ�����ڵ�ַ
 *      |__�û��ڹ����ļ��¶�����жϺ���������������Ϊ:�޷���ֵ,�޲���(eg. void isr(void);)
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 *
 */
uint8_t LPLD_PIT_SetIsr(PITx pitx, PIT_ISR_CALLBACK isr_func)
{
  PIT_ISR[pitx] = isr_func;
  intEnable(84+pitx);
  
  return 1;
}

/*
 * LPLD_PIT_ClearIsr
 * ���PITͨ���û�������жϷ��������������ж�
 * 
 * ����:
 *    pitx--PITģ���
 *      |__PIT0        --�����ж϶�ʱ��0
 *      |__PIT1        --�����ж϶�ʱ��0
 *      |__PIT2        --�����ж϶�ʱ��0
 *      |__PIT3        --�����ж϶�ʱ��0
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 *
 */
uint8_t LPLD_PIT_ClearIsr(PITx pitx)
{
    intEnable(84+pitx);
  PIT_ISR[pitx] = NULL;
  
  return 1;
}


/*
 * LPLD_PIT_Isr
 * PITͨ���жϵײ���ں���
 * 
 * �û������޸ģ������Զ������Ӧͨ���жϺ���
 */
void LPLD_PIT_Isr(void)
{
  #define PIT_VECTORNUM   (*(volatile uint8_t*)(0xE000ED04))
  uint8_t pit_ch = PIT_VECTORNUM - 84;
    
  //�����û��Զ����жϷ���
  PIT_ISR[pit_ch]();  
  //����жϱ�־λ
  PIT_TFLG_REG(PIT_BASE_PTR,pit_ch) |= PIT_TFLG_TIF_MASK;
}

void clear_pitflag(PITx pitx)
{
    PIT_TFLG_REG(PIT_BASE_PTR, pitx) = 1;
}

