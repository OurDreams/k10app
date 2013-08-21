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
#include <types.h>
#include <gpio.h>

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
     gpio_init(PORTB, 20, GPO, 1);
     /* 使能FLEXBUS的时钟 7-18 lzh mode */
     SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
	 /* 指令和数据读写使能 */
	 SIM_SOPT2 |= SIM_SOPT2_FBSL(3);
     /* 设置片选地址，高16位有效 */
     FB_CSAR0 = (uint32_t)CS0_START_ADDRESS;
	 /* 控制寄存器
	  * ASET(21 - 20): 地址有效之后几个CLOCK之后FB_CS有效
	  * WS(15 - 10)  : 应答等待时间
	  * BLS(9)       : 数据在AD的左还是右 0: left 1: right
	  * AA(8)        : AUTO ACKNOWLEDGE   0: disable acknowledge 1: enable acknowledge
	  * PS(7 - 6)    : 数据宽度 1: 8 byte port size
	 */
     FB_CSCR0 = FB_CSCR_ASET(1) | FB_CSCR_WS(1) | FB_CSCR_AA_MASK | FB_CSCR_PS(1);
	 /* 屏蔽地址寄存器 ，控制在基地址的基础上寻址的范围 高16位是屏蔽位
	  * BAM(31 - 16): 地址屏蔽寄存器位
	  * WP(8)       : 写屏蔽位 0: 可写 1: 写屏蔽
	  * V(0)        : 片选有效位 0: 片选无效 1: 片选有效
	  */
	 FB_CSMR0 = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
	 /* 引脚复用控制寄存器
	  * GROUP1(31-28): 1: FB_CS1
	  * GROUP2(27-24): 0: FB_CS4
	  * GROUP3(23-20): 0: FB_CS5
	  * GROUP4(19-16): 1: FB_CS2
	  * GROUP5(15-12): 1: FB_CS3
	  *
	 */
	 FB_CSPMCR = 0x10011000;

	 //fb clock divider 3
	 SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(0x0a);

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
 #if 0
 #define MRAM_START_ADDRESS	(*(vuint8*)(0x60000000))

/********************************************************************/
void hwflexbus_main (void)
{
    uint8 wdata8 = 0x00;
    uint8 rdata8 = 0x00;
    uint16 wdata16 = 0x00;
    uint16 rdata16 = 0x00;
    uint32 wdata32 = 0x00;
    uint32 rdata32 = 0x00;
    uint32 n = 0;

    printf("\n****MRAM Test****\n");

    printf("\nInitializing the FlexBus\n");

    #ifdef TWR_K40X256
      TWRK40_flexbus_init();
    #elif TWR_K60N512
      TWRK60_flexbus_init();
    #else
      printf("\nError - Board not defined \n");
    #endif

    //print FlexBus configuration register contents
    printf("FB_CSCR0 is %08x \n\r",FB_CSCR0);
    printf("FB_CSMR0 is %08x \n\r",FB_CSMR0);
    printf("FB_CSAR0 is %08x \n\r",FB_CSAR0);
    printf("FB_CSPMCR is %08x \n\r",FB_CSPMCR);
    printf("SIM_CLKDIV1 is %08x \n\r",SIM_CLKDIV1);

	printf("\nTesting 8-bit write/reads\n");

   	wdata8=0xA5;   //data to write to mram

	for(n=0x00000;n<0x000F;n++)  //address offset
	{
  		*(vuint8*)(&MRAM_START_ADDRESS + n) = wdata8;  //write
  		rdata8=0x00;  //clear data variable;
  		rdata8=(*(vuint8*)(&MRAM_START_ADDRESS + n));  //read
  		printf("ADDR: 0x%08x WRITE: 0x%02x READ: 0x%02x\n",&MRAM_START_ADDRESS + n,wdata8,rdata8);
	}

	printf("\nTesting 16-bit write/reads\n");

   	wdata16=0x1234;   //data to write to mram

	for(n=0x00010;n<0x001F;n+=2)  //address offset
	{
  		*(vuint16*)(&MRAM_START_ADDRESS + n) = wdata16;  //write
  		rdata16=0x00;  //clear data variable;
  		rdata16=(*(vuint16*)(&MRAM_START_ADDRESS + n));  //read
		printf("ADDR: 0x%08x WRITE: 0x%04x READ: 0x%04x\n",&MRAM_START_ADDRESS + n,wdata16,rdata16);
	}

	printf("\nTesting 32-bit write/reads\n");

   	wdata32=0x87654321;   //data to write to mram

	for(n=0x00020;n<0x002F;n+=4)  //address offset
	{
  		*(vuint32*)(&MRAM_START_ADDRESS + n) = wdata32;  //write
  		rdata32=0x00;  //clear data variable;
  		rdata32=(*(vuint32*)(&MRAM_START_ADDRESS + n));  //read
 		printf("ADDR: 0x%08x WRITE: 0x%08x READ: 0x%08x\n",&MRAM_START_ADDRESS + n,wdata32,rdata32);
	}

}
/********************************************************************/


void TWRK40_flexbus_init(void){

/* Enable the FlexBus                                               */
/* Configure the FlexBus Registers for 8-bit port size              */
/*  with multiplexed address and data using chip select 0           */
/* These configurations are specific to communicating with          */
/*  the MRAM used in this example                                   */
/* For K40 tower module - use the byte lane shift because there     */
/*  is a latch on the board which handles multiplexed address/data  */

  //Set Base address

  FB_CSAR0 = (uint32)&MRAM_START_ADDRESS;

  FB_CSCR0  =   FB_CSCR_BLS_MASK   // set byte lane shift for data on FB_AD[7:0] aka. right justified mode
              | FB_CSCR_PS(1)      // 8-bit port
              | FB_CSCR_AA_MASK    // auto-acknowledge
              | FB_CSCR_ASET(0x1)  // assert chip select on second clock edge after address is asserted
              | FB_CSCR_WS(0x1)    // 1 wait state - may need a wait state depending on the bus speed
              ;

  FB_CSMR0  =   FB_CSMR_BAM(0x7)  //Set base address mask for 512K address space
              | FB_CSMR_V_MASK    //Enable cs signal
              ;

  //enable BE signals - note, not used in this example
  FB_CSPMCR = 0x02200000;

  //fb clock divider 3
   SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(0x3);

  /* Configure the pins needed to FlexBus Function (Alt 5) */
  /* this example uses low drive strength settings         */
  //address/Data
  PORTA_PCR7=PORT_PCR_MUX(5);          //fb_ad[18]
  PORTA_PCR8=PORT_PCR_MUX(5);          //fb_ad[17]
  PORTA_PCR9=PORT_PCR_MUX(5);          //fb_ad[16]
  PORTA_PCR10=PORT_PCR_MUX(5);          //fb_ad[15]
  PORTA_PCR24=PORT_PCR_MUX(5);          //fb_ad[14]
  PORTA_PCR25=PORT_PCR_MUX(5);          //fb_ad[13]
  PORTA_PCR26=PORT_PCR_MUX(5);          //fb_ad[12]
  PORTA_PCR27=PORT_PCR_MUX(5);          //fb_ad[11]
  PORTA_PCR28=PORT_PCR_MUX(5);          //fb_ad[10]
  PORTD_PCR10=PORT_PCR_MUX(5);          //fb_ad[9]
  PORTD_PCR11=PORT_PCR_MUX(5);          //fb_ad[8]
  PORTD_PCR12=PORT_PCR_MUX(5);          //fb_ad[7]
  PORTD_PCR13=PORT_PCR_MUX(5);          //fb_ad[6]
  PORTD_PCR14=PORT_PCR_MUX(5);          //fb_ad[5]
  PORTE_PCR8=PORT_PCR_MUX(5);          //fb_ad[4]
  PORTE_PCR9=PORT_PCR_MUX(5);          //fb_ad[3]
  PORTE_PCR10=PORT_PCR_MUX(5);          //fb_ad[2]
  PORTE_PCR11=PORT_PCR_MUX(5);          //fb_ad[1]
  PORTE_PCR12=PORT_PCR_MUX(5);          //fb_ad[0]
  //control signals
  PORTA_PCR11=PORT_PCR_MUX(5);          //fb_oe_b
  PORTD_PCR15=PORT_PCR_MUX(5);          //fb_rw_b
  PORTE_PCR7=PORT_PCR_MUX(5);          //fb_cs0_b
  PORTE_PCR6=PORT_PCR_MUX(5);          //fb_ale
}

/**
 ******************************************************************************
 * @brief       hw_can_init: can模块初始化
 * @param[in]   None
 * @param[out]  None
 * @return      None
 *
 * @note        初始化can模块通道
 ******************************************************************************
 */
 void hw_flexbus_init(uint32 csaddr)
 {

     /* Enable the FlexBus                                               */
     /* Configure the FlexBus Registers for 8-bit port size              */
     /*  with separate address and data using chip select 0              */
     /* These configurations are specific to communicating with          */
     /*  the MRAM used in this example                                   */
     /* For K60 tower module - do not set byte lane shift so that data   */
     /*  comes out on AD[31:24]                                          */

     /* Set Base address */
     FB_CSAR0 = (uint32)&csaddr;

     /* set ps 1: 8 bit width
      * internal acknowledge
      * assert chip select on second clock edge after address is asserted
      * config how many clock cycle FB_TA is assert after FB_CS is assert
     */
     FB_CSCR0  =   FB_CSCR_PS(1)      // 8-bit port
               | FB_CSCR_AA_MASK      // auto-acknowledge
               | FB_CSCR_ASET(0x1)    // assert chip select on second clock edge after address is asserted
               | FB_CSCR_WS(0x1)      // 1 wait state - may need a wait state depending on the bus speed
               ;
     /* enable cs ;enabe read write */
     FB_CSMR0  =   FB_CSMR_BAM(0x7)  //Set base address mask for 512K address space  ??? 如何算出来的
               | FB_CSMR_V_MASK      //Enable cs signal
               ;

     /* enable BE signals - note, not used in this example */
     FB_CSPMCR = 0x02200000;

     /* fb clock divider 3  bus clock */
     SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(0x3);


     /* Configure the pins needed to FlexBus Function (Alt 5) */
     /* this example uses low drive strength settings         */

     /* address */
     PORTB_PCR11 = PORT_PCR_MUX(5);           //  fb_ad[18]
     PORTB_PCR16 = PORT_PCR_MUX(5);           //  fb_ad[17]
     PORTB_PCR17 = PORT_PCR_MUX(5);           //  fb_ad[16]
     PORTB_PCR18 = PORT_PCR_MUX(5);           //  fb_ad[15]
     PORTC_PCR0  = PORT_PCR_MUX(5);           //  fb_ad[14]
     PORTC_PCR1  = PORT_PCR_MUX(5);           //  fb_ad[13]
     PORTC_PCR2  = PORT_PCR_MUX(5);           //  fb_ad[12]
     PORTC_PCR4  = PORT_PCR_MUX(5);           //  fb_ad[11]
     PORTC_PCR5  = PORT_PCR_MUX(5);           //  fb_ad[10]
     PORTC_PCR6  = PORT_PCR_MUX(5);           //  fb_ad[9]
     PORTC_PCR7  = PORT_PCR_MUX(5);           //  fb_ad[8]
     PORTC_PCR8  = PORT_PCR_MUX(5);           //  fb_ad[7]
     PORTC_PCR9  = PORT_PCR_MUX(5);           //  fb_ad[6]
     PORTC_PCR10 = PORT_PCR_MUX(5);           //  fb_ad[5]
     PORTD_PCR2  = PORT_PCR_MUX(5);           //  fb_ad[4]
     PORTD_PCR3  = PORT_PCR_MUX(5);           //  fb_ad[3]
     PORTD_PCR4  = PORT_PCR_MUX(5);           //  fb_ad[2]
     PORTD_PCR5  = PORT_PCR_MUX(5);           //  fb_ad[1]
     PORTD_PCR6  = PORT_PCR_MUX(5);           //  fb_ad[0]

     /* data */
     PORTB_PCR20 = PORT_PCR_MUX(5);           //  fb_ad[31] used as d[7]
     PORTB_PCR21 = PORT_PCR_MUX(5);           //  fb_ad[30] used as d[6]
     PORTB_PCR22 = PORT_PCR_MUX(5);           //  fb_ad[29] used as d[5]
     PORTB_PCR23 = PORT_PCR_MUX(5);           //  fb_ad[28] used as d[4]
     PORTC_PCR12 = PORT_PCR_MUX(5);           //  fb_ad[27] used as d[3]
     PORTC_PCR13 = PORT_PCR_MUX(5);           //  fb_ad[26] used as d[2]
     PORTC_PCR14 = PORT_PCR_MUX(5);           //  fb_ad[25] used as d[1]
     PORTC_PCR15 = PORT_PCR_MUX(5);           //  fb_ad[24] used as d[0]

     /* control signals */
     PORTB_PCR19 = PORT_PCR_MUX(5);          // fb_oe_b
     PORTC_PCR11 = PORT_PCR_MUX(5);          // fb_rw_b
     PORTD_PCR1  = PORT_PCR_MUX(5);          // fb_cs0_b
     PORTD_PCR0  = PORT_PCR_MUX(5);          // fb_ale

 }
 #endif
 /*-------------------------------- end of hw_flexbus.c --------------------------------*/
