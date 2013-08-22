/*
 * @file     hw_flexbus.c
 * @brief    GPIO�ĵײ�̼���
 * @version  V0.0.1
 * @date     2013.5.27
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013.5.27      ������       ��һ��
 *
 */

 /*-----------------------------------------------------------------------------
 Section: Include
------------------------------------------------------------------------------*/
 #include <flexbus.h>

/*------------------------------------------------------------------------------
Section: Pub Functions
------------------------------------------------------------------------------*/

/**
 ******************************************************************************
 * @brief       hw_flexbus_init: FLEXBUS ��ʼ������
 * @param[in]   None
 * @return      None
 *
 * @note        ��ʼ��FLEXBUS���ϲ����, FLEXBUS�ĵ�ַ��Χ��0xA000_0000 -- 0xDFFF_FFFF
                ʱ��ԴΪϵͳʱ�ӡ�
                ����: 1. ���ߴ�����δ���
                      2. ����ģʽ�������
 ******************************************************************************
 */
 void hw_flexbus_init(void)
 {
     /* ʹ��FLEXBUS��ʱ�� 7-18 lzh mode */
     SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
     /* ָ������ݶ�дʹ�� */
     SIM_SOPT2 |= SIM_SOPT2_FBSL(3);

     /* ����Ƭѡ��ַ����16λ��Ч */

     FB_CSAR0  =  (uint32_t)CS0_START_ADDRESS;

     FB_CSAR1 = (uint32_t)CS1_START_ADDRESS;
     FB_CSAR2 = (uint32_t)CS2_START_ADDRESS;
     FB_CSAR4 = (uint32_t)CS4_START_ADDRESS;
     //#if PRD_TYPE == PRD_LCD
     FB_CSAR5 = (uint32_t)CS5_START_ADDRESS;
    // #endif
     /* ���ƼĴ���
      * ASET(21 - 20): ��ַ��Ч֮�󼸸�CLOCK֮��FB_CS��Ч
      * WS(15 - 10)  : Ӧ��ȴ�ʱ��
      * BLS(9)       : ������AD�������� 0: left 1: right
      * AA(8)        : AUTO ACKNOWLEDGE   0: disable acknowledge 1: enable acknowledge
      * PS(7 - 6)    : ���ݿ�� 1: 8 byte port size
     */
     FB_CSCR0 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     FB_CSCR1 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     FB_CSCR2 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     FB_CSCR4 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     //#if PRD_TYPE == PRD_LCD
     FB_CSCR5 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     //#endif
     /* ���ε�ַ�Ĵ��� �������ڻ���ַ�Ļ�����Ѱַ�ķ�Χ ��16λ������λ
      * BAM(31 - 16): ��ַ���μĴ���λ
      * WP(8)       : д����λ 0: ��д 1: д����
      * V(0)        : Ƭѡ��Чλ 0: Ƭѡ��Ч 1: Ƭѡ��Ч
      */
     FB_CSMR0 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     FB_CSMR1 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     FB_CSMR2 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     FB_CSMR4 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     //#if PRD_TYPE == PRD_LCD
     FB_CSMR5 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     //#endif
     /* ���Ÿ��ÿ��ƼĴ���
      * GROUP1(31-28): 1: FB_CS1
      * GROUP2(27-24): 0: FB_CS4
      * GROUP3(23-20): 0: FB_CS5
      * GROUP4(19-16): 1: FB_CS2
      * GROUP5(15-12): 1: FB_CS3
      *
     */
     FB_CSPMCR = 0x10011000;
     /* FB_AD31 - FB_AD24 */
     PORTB_PCR20 = PORT_PCR_MUX(5);
     PORTB_PCR21 = PORT_PCR_MUX(5);
     PORTB_PCR22 = PORT_PCR_MUX(5);
     PORTB_PCR23 = PORT_PCR_MUX(5);
     PORTC_PCR12 = PORT_PCR_MUX(5);
     PORTC_PCR13 = PORT_PCR_MUX(5);
     PORTC_PCR14 = PORT_PCR_MUX(5);
     PORTC_PCR15 = PORT_PCR_MUX(5);
     /* FB_CS0_b FB_CS1_b FB_CS2_b FB_CS4_b Ƭѡ���� */
     PORTD_PCR1 = PORT_PCR_MUX(5);
     PORTD_PCR0 = PORT_PCR_MUX(5);
     PORTC_PCR18 = PORT_PCR_MUX(5);
     PORTC_PCR17 = PORT_PCR_MUX(5);
     /* ң�ذ��õ��� PORTB_PCR19: FB_OE_b PORTC_PCR11: FB_R/W_b */
     PORTB_PCR19 = PORT_PCR_MUX(5);
     PORTC_PCR11 = PORT_PCR_MUX(5);
 }

 /*-------------------------------- end of hw_flexbus.c --------------------------------*/
