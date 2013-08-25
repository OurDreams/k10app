
#include <stdio.h>
#include <flash.h>

void Flash_init(void)
{
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;       // 清除Flash预读取缓冲区

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // 等待命令完成

    FTFL_FSTAT = (0 | FTFL_FSTAT_ACCERR_MASK      // 清除访问错误标志位
                    | FTFL_FSTAT_FPVIOL_MASK);
}


uint8_t Flash_erase_sector(uint16_t sectorNo)
{
    union
    {
        uint32_t word;
        uint8_t byte[4];
    } dest;

    dest.word = (uint32_t)(sectorNo * (1 << 11));

    FTFL_FCCOB0 = 0x09; //擦除扇区

    // 设置目标地址
    FTFL_FCCOB1 = dest.byte[2];
    FTFL_FCCOB2 = dest.byte[1];
    FTFL_FCCOB3 = dest.byte[0];

    // 执行命令序列
    if (1 == Flash_cmd_launch()) //若执行命令出现错误
    {
        printf("bug[%d]\n", __LINE__);
        return 1; //擦除命令错误
    }

    // 若擦除sector0时，则解锁设备
    if (dest.word <= 0x800)
    {

        FTFL_FCCOB0 = 0x06; // 写入4字节
        // 设置目标地址
        FTFL_FCCOB1 = 0x00;
        FTFL_FCCOB2 = 0x04;
        FTFL_FCCOB3 = 0x0C;
        // 数据
        FTFL_FCCOB4 = 0xFF;
        FTFL_FCCOB5 = 0xFF;
        FTFL_FCCOB6 = 0xFF;
        FTFL_FCCOB7 = 0xFE;
        // 执行命令序列
        if (1 == Flash_cmd_launch()) //若执行命令出现错误
        {
            printf("bug[%d]\n", __LINE__);
            return 2; //解锁命令错误
        }
    }

    return 0; //成功返回
}



uint8_t Flash_read(uint16_t destaddr, uint16_t cnt, uint8_t *bBuf)
{
    uint32_t wAddr = 0;
    wAddr = destaddr;
    while (cnt--)
    {
        *bBuf++ = *(uint8_t*)wAddr++;
    }
    return TRUE;
}


uint8_t Flash_write(uint32_t destaddr, uint16_t cnt, uint8_t buf[])
{
    union
    {
        uint32_t word;
        uint8_t byte[4];
    } dest;

    if ((destaddr & 0x03) || (cnt & 0x03))
    {
        return 1; //参数设定错误，偏移量未对齐（4字节对齐）
    }
    dest.word = destaddr;

    // 设置写入命令
    FTFL_FCCOB0 = 0x06;
    for (uint16_t size = 0; size < cnt; size += 4)
    {
        // 设置目标地址
        FTFL_FCCOB1 = dest.byte[2];
        FTFL_FCCOB2 = dest.byte[1];
        FTFL_FCCOB3 = dest.byte[0];

        // 拷贝数据
        FTFL_FCCOB4 = buf[3];
        FTFL_FCCOB5 = buf[2];
        FTFL_FCCOB6 = buf[1];
        FTFL_FCCOB7 = buf[0];

        if (1 == Flash_cmd_launch())
        {
            return 2; //写入命令错误
        }
        buf += 4;
    }

    return 0; //成功执行
}


uint32_t Flash_cmd_launch(void)
{
    // 清除访问错误标志位和非法访问标志位
    FTFL_FSTAT = (1 << 5) | (1 << 4);

    // 启动命令
    FTFL_FSTAT = (1 << 7);

    // 等待命令结束
    while (!(FTFL_FSTAT & (1 << 7)))
    {
        ;
    }

    // 检查错误标志
    if (FTFL_FSTAT & ((1 << 5) | (1 << 4) | 1))
    {
        return 1; //执行命令出错
    }

    return 0; //执行命令成功
}
