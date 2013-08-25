/*
 * --------------"拉普兰德K60底层库"-----------------
 *
 * 测试硬件平台:LPLD_K60 Card
 * 版权所有:北京拉普兰德电子技术有限公司
 * 网络销售:http://laplenden.taobao.com
 * 公司门户:http://www.lpld.cn
 *
 * 文件名: HAL_FLASH.c
 * 用途: 用于配置K60 FLASH
 * 最后修改日期: 20120730
 *
 * 开发者使用协议:
 *  本代码面向所有使用者开放源代码，开发者可以随意修改源代码。但本段及以上注释应
 *  予以保留，不得更改或删除原版权所有者姓名。二次开发者可以加注二次版权所有者，
 *  但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 */

#include "flash.h"
#include <stdio.h>
/*
 * LPLD_Flash_Init
 * Flash模块初始化
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_Flash_Init(void)
{
    //检查Flash访问错误
    if (FTFL_FSTAT & FTFL_FSTAT_ACCERR_MASK)
    {
      FTFL_FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //清除错误标志
    }
    //检查保护错误
    else if (FTFL_FSTAT & FTFL_FSTAT_FPVIOL_MASK)
    {
      FTFL_FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
    }
    //检查读冲突错误
    else if (FTFL_FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
    {
      FTFL_FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
    }

    //禁用Flash模块的数据缓存
    FMC_PFB0CR &= ~FMC_PFB0CR_B0DCE_MASK;
    FMC_PFB1CR &= ~FMC_PFB1CR_B1DCE_MASK;
} 

/*
 * LPLD_Flash_SectorErase
 * 擦除Flash扇区
 * 
 * 参数:
 *    FlashPtr--扇区地址指针，即扇区号乘以2048
 *
 * 输出:
 *    错误代码
 */
uint8_t LPLD_Flash_SectorErase(uint32_t FlashPtr)
{
    uint8_t Return = FLASH_OK;

    //提示flash地址指向保护区域，是否要请标志呢？戴旭毅
    FTFL_FSTAT |= (FTFL_FSTAT_FPVIOL_MASK | FTFL_FSTAT_ACCERR_MASK);

     //等待CCIF置1
     while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));
     for (volatile int i = 0; i < 10; i++);

     //写入命令道FCCOB寄存器
     FTFL_FCCOB0 = FLASH_CMD_ERSSCR;
     FTFL_FCCOB1 = (uint8_t) (FlashPtr >> 16);
     FTFL_FCCOB2 = (uint8_t)((FlashPtr >> 8 ) & 0xFF);
     FTFL_FCCOB3 = (uint8_t)( FlashPtr & 0xFF);

     //执行命令
     FTFL_FSTAT |= FTFL_FSTAT_CCIF_MASK;
     //等待命令完成
     while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));
     for (volatile int i = 0; i < 10; i++);

     //检查Flash访问错误
     if (FTFL_FSTAT & FTFL_FSTAT_ACCERR_MASK)
     {
       FTFL_FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //清除错误标志
       Return |= FLASH_FACCERR;                    //更新返回值
     }
     //检查Flash保护标志
     else if (FTFL_FSTAT & FTFL_FSTAT_FPVIOL_MASK)
     {
       FTFL_FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
       Return |= FLASH_FPVIOL;
     }
     //检查Flash读冲突错误
     else if (FTFL_FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
     {
       FTFL_FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
       Return |= FLASH_RDCOLERR;
     }
     //检查MGSTAT0非可纠正错误
     else if (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0_MASK)
     {
       Return |= FLASH_MGSTAT0;
     }

     return  Return;
}

/*
 * LPLD_Flash_ByteProgram
 * 编程Flash
 * 
 * 参数:
 *    FlashStartAdd--Flash编程起始地址
 *    *DataSrcPtr--数据源指针
 *    NumberOfBytes--数据字节长度
 *
 * 输出:
 *    错误代码
 */
uint8_t LPLD_Flash_ByteProgram(uint32_t FlashStartAdd, uint32_t *DataSrcPtr, uint32_t NumberOfBytes)
{
    uint8_t Return = FLASH_OK;
    
    uint32_t size_buffer;
    
    if (NumberOfBytes == 0)
    {
      return FLASH_CONTENTERR;
    }
    else
    {
      size_buffer = (NumberOfBytes - 1)/4 + 1;
    }

    //提示flash地址指向保护区域，是否要请标志呢？戴旭毅
    FTFL_FSTAT |= (FTFL_FSTAT_FPVIOL_MASK | FTFL_FSTAT_ACCERR_MASK);

    //等待直到CCIF设置
    while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK)){};

    while ((size_buffer) && (Return == FLASH_OK))
    {
      //写FLASH命令到FCCOB
      FTFL_FCCOB0 = FLASH_CMD_PGM4;
      FTFL_FCCOB1 = (uint8_t)(FlashStartAdd >> 16);
      FTFL_FCCOB2 = (uint8_t)((FlashStartAdd >> 8) & 0xFF);
      FTFL_FCCOB3 = (uint8_t)(FlashStartAdd & 0xFF);
      //如果K60设置成为小端格式，进行如下操作
      FTFL_FCCOB4 = (uint8_t)(*((uint8_t*)DataSrcPtr+3));
      FTFL_FCCOB5 = (uint8_t)(*((uint8_t*)DataSrcPtr+2));
      FTFL_FCCOB6 = (uint8_t)(*((uint8_t*)DataSrcPtr+1));
      FTFL_FCCOB7 = (uint8_t)(*((uint8_t*)DataSrcPtr+0));
      //如果K60设置成为大端格式，进行如下操作
      //FTFL_FCCOB4 = (uint8_t)(*((uint_8*)DataSrcPtr+0));
      //FTFL_FCCOB5 = (uint8_t)(*((uint_8*)DataSrcPtr+1));
      //FTFL_FCCOB6 = (uint8_t)(*((uint_8*)DataSrcPtr+2));
      //FTFL_FCCOB7 = (uint8_t)(*((uint_8*)DataSrcPtr+3));
      //设置FLASH加载命令
      FTFL_FSTAT |= FTFL_FSTAT_CCIF_MASK;
      //等待命令完成
      while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK)) {};

      //检测FLASH访问错误
      if (FTFL_FSTAT & FTFL_FSTAT_ACCERR_MASK)
      {

          FTFL_FSTAT |= FTFL_FSTAT_ACCERR_MASK;

          Return |= FLASH_FACCERR;
      }
      //检测FLASH保护错误
      else if (FTFL_FSTAT & FTFL_FSTAT_FPVIOL_MASK)
      {
          FTFL_FSTAT |= FTFL_FSTAT_FPVIOL_MASK;

          Return |= FLASH_FPVIOL;
      }

      else if (FTFL_FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
      {
          FTFL_FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;

          Return |= FLASH_RDCOLERR;
      }
      else if (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0_MASK)
      {
          Return |= FLASH_MGSTAT0;
      }
      size_buffer --;
      (uint32_t*)DataSrcPtr++;
      FlashStartAdd +=4;
    } 
    return  Return;
} 
