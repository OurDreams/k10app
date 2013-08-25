/*
 * --------------"拉普兰德K60底层库"-----------------
 *
 * 测试硬件平台:LPLD_K60 Card
 * 版权所有:北京拉普兰德电子技术有限公司
 * 网络销售:http://laplenden.taobao.com
 * 公司门户:http://www.lpld.cn
 *
 * 文件名: HAL_PIT.c
 * 用途: PIT底层模块相关函数
 * 最后修改日期: 20120321
 *
 * 开发者使用协议:
 *  本代码面向所有使用者开放源代码，开发者可以随意修改源代码。但本段及以上注释应
 *  予以保留，不得更改或删除原版权所有者姓名。二次开发者可以加注二次版权所有者，
 *  但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 */

/*
 *******需用到定时中断，请在isr.h中粘贴一下代码:*********

//PIT模块中断服务定义
#undef  VECTOR_084
#define VECTOR_084 LPLD_PIT_Isr
#undef  VECTOR_085
#define VECTOR_085 LPLD_PIT_Isr
#undef  VECTOR_086
#define VECTOR_086 LPLD_PIT_Isr
#undef  VECTOR_087
#define VECTOR_087 LPLD_PIT_Isr
//以下函数在LPLD_Kinetis底层包，不必修改
extern void LPLD_PIT_Isr(void);

 ***********************代码结束*************************
*/
#include <MK10DZ10.h>
#include <pit.h>
#include <types.h>
#include <stddef.h>
#include <intLib.h>
#include <stdio.h>

//引用总线时钟频率，该值在sysinit.c文件中取得
extern uint32_t core_clk_mhz;

//用户自定义中断服务函数数组
PIT_ISR_CALLBACK PIT_ISR[4];

/*
 * LPLD_PIT_Init
 * PIT通用初始化函数
 * 
 * 参数:
 *    pitx--PIT模块号
 *      |__PIT0        --周期中断定时器0
 *      |__PIT1        --周期中断定时器0
 *      |__PIT2        --周期中断定时器0
 *      |__PIT3        --周期中断定时器0
 *    period_us--中断周期
 *      |__单位为微妙
 *    isr_func--用户中断程序入口地址
 *      |__用户在工程文件下定义的中断函数名，函数必须为:无返回值,无参数(eg. void isr(void);)
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8_t LPLD_PIT_Init(PITx pitx, uint32_t period_us, PIT_ISR_CALLBACK isr_func)
{ 
  uint32_t ldval;
//  uint32_t core_clk_mhz;
  
  
  //计算定时加载值
//  core_clk_mhz = periph_clk_khz/1000;
  ldval = period_us*core_clk_mhz;

  
  //开启定时模块时钟
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
  
  // 开启 PIT
  PIT_MCR = 0x00;
#if 0
  if(isr_func!=NULL){
    PIT_ISR[pitx] = isr_func;
#endif
    intConnect(84+pitx, isr_func, 0);
    //在NVIC中使能PIT中断
    intEnable(84+pitx);
 // }
  
  //period = (period_ns/bus_period_ns)-1
  PIT_LDVAL_REG(PIT_BASE_PTR,pitx) = ldval-1;
  
  //使能中断
  PIT_TCTRL_REG(PIT_BASE_PTR,pitx) = PIT_TCTRL_TIE_MASK;
  //开始定时
  PIT_TCTRL_REG(PIT_BASE_PTR,pitx) |= PIT_TCTRL_TEN_MASK;
  
  return 1;
}

/*
 * LPLD_PIT_SetIsr
 * 设置PIT通道用户定义的中断服务函数，并使能中断
 * 
 * 参数:
 *    pitx--PIT模块号
 *      |__PIT0        --周期中断定时器0
 *      |__PIT1        --周期中断定时器0
 *      |__PIT2        --周期中断定时器0
 *      |__PIT3        --周期中断定时器0
 *    isr_func--用户中断程序入口地址
 *      |__用户在工程文件下定义的中断函数名，函数必须为:无返回值,无参数(eg. void isr(void);)
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
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
 * 清除PIT通道用户定义的中断服务函数，并禁用中断
 * 
 * 参数:
 *    pitx--PIT模块号
 *      |__PIT0        --周期中断定时器0
 *      |__PIT1        --周期中断定时器0
 *      |__PIT2        --周期中断定时器0
 *      |__PIT3        --周期中断定时器0
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
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
 * PIT通用中断底层入口函数
 * 
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void LPLD_PIT_Isr(void)
{
  #define PIT_VECTORNUM   (*(volatile uint8_t*)(0xE000ED04))
  uint8_t pit_ch = PIT_VECTORNUM - 84;
    
  //调用用户自定义中断服务
  PIT_ISR[pit_ch]();  
  //清除中断标志位
  PIT_TFLG_REG(PIT_BASE_PTR,pit_ch) |= PIT_TFLG_TIF_MASK;
}

void clear_pitflag(PITx pitx)
{
    PIT_TFLG_REG(PIT_BASE_PTR, pitx) = 1;
}

