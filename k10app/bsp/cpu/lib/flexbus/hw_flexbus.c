/*
 * @file     hw_flexbus.c
 * @brief    GPIO的底层固件库
 * @version  V0.0.1
 * @date     2013.5.27
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013.5.27      梁增辉       第一版
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
 * @brief       hw_flexbus_init: FLEXBUS 初始化函数
 * @param[in]   None
 * @return      None
 *
 * @note        初始化FLEXBUS供上层调用, FLEXBUS的地址范围是0xA000_0000 -- 0xDFFF_FFFF
                时钟源为系统时钟。
                问题: 1. 总线错误如何处理
                      2. 工作模式如何配置
 ******************************************************************************
 */
 void hw_flexbus_init(void)
 {
     /* 使能FLEXBUS的时钟 7-18 lzh mode */
     SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
     /* 指令和数据读写使能 */
     SIM_SOPT2 |= SIM_SOPT2_FBSL(3);

     /* 设置片选地址，高16位有效 */

     FB_CSAR0  =  (uint32_t)CS0_START_ADDRESS;

     FB_CSAR1 = (uint32_t)CS1_START_ADDRESS;
     FB_CSAR2 = (uint32_t)CS2_START_ADDRESS;
     FB_CSAR4 = (uint32_t)CS4_START_ADDRESS;
     //#if PRD_TYPE == PRD_LCD
     FB_CSAR5 = (uint32_t)CS5_START_ADDRESS;
    // #endif
     /* 控制寄存器
      * ASET(21 - 20): 地址有效之后几个CLOCK之后FB_CS有效
      * WS(15 - 10)  : 应答等待时间
      * BLS(9)       : 数据在AD的左还是右 0: left 1: right
      * AA(8)        : AUTO ACKNOWLEDGE   0: disable acknowledge 1: enable acknowledge
      * PS(7 - 6)    : 数据宽度 1: 8 byte port size
     */
     FB_CSCR0 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     FB_CSCR1 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     FB_CSCR2 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     FB_CSCR4 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     //#if PRD_TYPE == PRD_LCD
     FB_CSCR5 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
     //#endif
     /* 屏蔽地址寄存器 ，控制在基地址的基础上寻址的范围 高16位是屏蔽位
      * BAM(31 - 16): 地址屏蔽寄存器位
      * WP(8)       : 写屏蔽位 0: 可写 1: 写屏蔽
      * V(0)        : 片选有效位 0: 片选无效 1: 片选有效
      */
     FB_CSMR0 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     FB_CSMR1 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     FB_CSMR2 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     FB_CSMR4 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     //#if PRD_TYPE == PRD_LCD
     FB_CSMR5 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
     //#endif
     /* 引脚复用控制寄存器
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
     /* FB_CS0_b FB_CS1_b FB_CS2_b FB_CS4_b 片选引脚 */
     PORTD_PCR1 = PORT_PCR_MUX(5);
     PORTD_PCR0 = PORT_PCR_MUX(5);
     PORTC_PCR18 = PORT_PCR_MUX(5);
     PORTC_PCR17 = PORT_PCR_MUX(5);
     /* 遥控板用到了 PORTB_PCR19: FB_OE_b PORTC_PCR11: FB_R/W_b */
     PORTB_PCR19 = PORT_PCR_MUX(5);
     PORTC_PCR11 = PORT_PCR_MUX(5);
 }

 /*-------------------------------- end of hw_flexbus.c --------------------------------*/
