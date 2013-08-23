/**
 ******************************************************************************
 * @file       flash_test.c
 * @version    V1.1.4
 * @brief      API C source file of flash_test.c
 *             Created on: 2013-7-8
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C), 2008-2012,Sanxing Electric Co.,Ltd.
 *
 ******************************************************************************
 */
#include <shell.h>
#include <flash.h>
#include <stdio.h>
#if 1
uint32_t flash_write (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    uint32_t pdata[] = {8,1,2,3,4,5,6,7,8,9,10,11};//48字节
    uint8_t Return;
#if 0
    for(uint32_t i = 0; i < sizeof pdata; i += 4)
    {
        Return = LPLD_Flash_ByteProgram(0x8000 + i, &pdata[i / 4], 4);
    }
#endif
    //是不是连续写4字节 有问题呢？戴旭毅
    Return = LPLD_Flash_ByteProgram(0x8000 , pdata, sizeof pdata);
    if (FLASH_OK != Return)
    {
        printf("fail flash write\r\n");
        switch (Return)
        {
        case 0x01:
            printf("error FLASH_FACCERR\r\n");
            break;
        case 0x02:
            printf("error FLASH_FPVIOL\r\n");
            break;
        case 0x08:
            printf("error FLASH_RDCOLERR\r\n");
            break;
        case 0x04:
            printf("error FLASH_MGSTAT0\r\n");
            break;
        default:break;
        }

    }
    else
    {
        printf("write ok\r\n");
    }
    return Return;
}
#endif

#if 0
/**
 ******************************************************************************
 * @brief   修改片内Flash指定区域的内容
 * @param[in]  *pdata   : 写入内容
 * @param[in]  address  : 目标地址
 * @param[in]  size     : 写入长度
 *
 * @retval  TRUE    : 成功
 * @retval  FALSE   : 失败
 ******************************************************************************
 */
bool_e
iflash_write(const uint8_t *pdata,
        uint32_t address,
        uint32_t size)
{
    for (uint32_t i = 0u; i < size; i += 4u)
    {
        /* 写入4字节 */
        if (FLASH_OK != LPLD_Flash_ByteProgram(address + i, (uint32_t *)(pdata) + i, 4))
        {
            return (FALSE);
        }
        /* 校验写入是否成功 */
        if (*(uint32_t *)(address + i) != *(const uint32_t *)(pdata) + i)
        {
            return (FALSE);
        }
    }
    return (TRUE);
}
uint32_t flash_write (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    uint32_t pdata[] = {0,1,2,3,4,5,6,7,8,9,10,11};//48字节
    if (TRUE != iflash_write((uint8_t *)pdata, 16*2048, sizeof pdata))
    {
        printf("write flash error\r\n");
    }
    if (TRUE != iflash_write((uint8_t *)pdata, 17*2048, sizeof pdata))
    {
        printf("write flash error\r\n");
    }
    return 0;
}
#endif
SHELL_CMD(
    fwrite,  CFG_MAXARGS,        flash_write,
    "write flash at 0x8000\r\n"
);
uint32_t flash_read (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    uint32_t pdata[12];
    uint32_t *address = (uint32_t *)0x8000;
    for (uint32_t i = 0u; i < sizeof pdata; i++)
    {
        pdata[i] = *(address);
        address ++;
        if((i % 12 == 0) && (i > 0))
        {
            printf("\n");
        }
        printf(" %d", pdata[i]);

    }
    printf("\n");

    address = (uint32_t *)0x8800;
    for (uint32_t j = 0u; j < sizeof pdata; j++)
    {
        pdata[j] = *(address);
        address ++;
        if((j % 12 == 0) && (j > 0))
        {
            printf("\n");
        }
        printf(" %d", pdata[j]);
    }
    printf("\n");

    return 0;
}

SHELL_CMD(
    fread,  CFG_MAXARGS,        flash_read,
    "read flash at 0x8000\r\n"
);
#if 0
uint32_t flash_erase (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    LPLD_Flash_SectorErase(0x8000);
    return 0;
}
#endif

/**
 ******************************************************************************
 * @brief   擦除片内Flash指定区域的内容
 * @param[in]  address  : 擦除地址
 * @param[in]  size     : 擦除长度
 *
 * @retval  TRUE    : 成功
 * @retval  FALSE   : 失败
 ******************************************************************************
 */
#if 1
bool_e
iflash_erase(uint32_t address,
        uint32_t size)
{
    uint8_t i;
    for (i = 16; i < 16 + (size + 2048) / 2048 ; i++)
    {
        if (FLASH_OK != LPLD_Flash_SectorErase(i * 2048))
        {
            return FALSE;
        }
    }

    return TRUE;
}
uint32_t flash_erase (cmd_tbl_t * cmdtp, uint32_t argc, const uint8_t *argv[])
{
    uint8_t Return;
    Return = iflash_erase(16*2048, 2*2048);
//    uint32_t size = 255 * 2048;
//    LPLD_Flash_Init();
#if 0
    for (uint32_t i = 15; i < 16 + (size + 2048) / 2048 ; i++)
    {
        if (FLASH_OK != LPLD_Flash_SectorErase(i * 2048))
        {
            printf("bug[%d]\n", i);
        }
    }
#endif
//    Return = LPLD_Flash_SectorErase(16 * 2048);
    Return = LPLD_Flash_SectorErase(17 * 2048);
    if (FLASH_OK != Return)
    {
        printf("fail flash erase\r\n");
        switch (Return)
        {
        case 0x01:
            printf("error FLASH_FACCERR\r\n");
            break;
        case 0x02:
            printf("error FLASH_FPVIOL\r\n");
            break;
        case 0x08:
            printf("error FLASH_RDCOLERR\r\n");
            break;
        case 0x04:
            printf("error FLASH_MGSTAT0\r\n");
            break;
        default:break;
        }

    }
    return 0;
}
#endif
SHELL_CMD(
    ferase,  CFG_MAXARGS,        flash_erase,
    "erase flash at 0x8000\r\n"
);

