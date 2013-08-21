/*
 * --------------"拉普兰德K60底层库"-----------------
 *
 * 测试硬件平台:LPLD_K60 Card
 * 版权所有:北京拉普兰德电子技术有限公司
 * 网络销售:http://laplenden.taobao.com
 * 公司门户:http://www.lpld.cn
 *
 * 文件名: HAL_FLASH.h
 * 用途: 用于配置K60 FLASH
 * 最后修改日期: 20120730
 *
 * 开发者使用协议:
 *  本代码面向所有使用者开放源代码，开发者可以随意修改源代码。但本段及以上注释应
 *  予以保留，不得更改或删除原版权所有者姓名。二次开发者可以加注二次版权所有者，
 *  但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 */

#ifndef _HAL_FLASH_H_
#define _HAL_FLASH_H_
#include "MK10DZ10.h"

/* 错误代码定义 */
#define FLASH_OK                    0x00
#define FLASH_FACCERR               0x01
#define FLASH_FPVIOL                0x02
#define FLASH_MGSTAT0               0x04
#define FLASH_RDCOLERR              0x08
#define FLASH_NOT_ERASED            0x10
#define FLASH_CONTENTERR            0x11

/* flash命令 */
#define FLASH_CMD_PGM4              0x06      //编程长字命令
#define FLASH_CMD_ERSSCR            0x09      //擦除Flash扇区命令


void LPLD_Flash_Init(void);
uint8_t LPLD_Flash_SectorErase(uint32_t);
uint8_t LPLD_Flash_ByteProgram(uint32_t, uint32_t*, uint32_t);

#endif /*_FLASH_FTFL_H_*/

