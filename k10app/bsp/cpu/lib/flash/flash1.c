
#include <stdio.h>
#include <flash.h>

void Flash_init(void)
{
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;       // ���FlashԤ��ȡ������

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFL_FSTAT = (0 | FTFL_FSTAT_ACCERR_MASK      // ������ʴ����־λ
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

    FTFL_FCCOB0 = 0x09; //��������

    // ����Ŀ���ַ
    FTFL_FCCOB1 = dest.byte[2];
    FTFL_FCCOB2 = dest.byte[1];
    FTFL_FCCOB3 = dest.byte[0];

    // ִ����������
    if (1 == Flash_cmd_launch()) //��ִ��������ִ���
    {
        printf("bug[%d]\n", __LINE__);
        return 1; //�����������
    }

    // ������sector0ʱ��������豸
    if (dest.word <= 0x800)
    {

        FTFL_FCCOB0 = 0x06; // д��4�ֽ�
        // ����Ŀ���ַ
        FTFL_FCCOB1 = 0x00;
        FTFL_FCCOB2 = 0x04;
        FTFL_FCCOB3 = 0x0C;
        // ����
        FTFL_FCCOB4 = 0xFF;
        FTFL_FCCOB5 = 0xFF;
        FTFL_FCCOB6 = 0xFF;
        FTFL_FCCOB7 = 0xFE;
        // ִ����������
        if (1 == Flash_cmd_launch()) //��ִ��������ִ���
        {
            printf("bug[%d]\n", __LINE__);
            return 2; //�����������
        }
    }

    return 0; //�ɹ�����
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
        return 1; //�����趨����ƫ����δ���루4�ֽڶ��룩
    }
    dest.word = destaddr;

    // ����д������
    FTFL_FCCOB0 = 0x06;
    for (uint16_t size = 0; size < cnt; size += 4)
    {
        // ����Ŀ���ַ
        FTFL_FCCOB1 = dest.byte[2];
        FTFL_FCCOB2 = dest.byte[1];
        FTFL_FCCOB3 = dest.byte[0];

        // ��������
        FTFL_FCCOB4 = buf[3];
        FTFL_FCCOB5 = buf[2];
        FTFL_FCCOB6 = buf[1];
        FTFL_FCCOB7 = buf[0];

        if (1 == Flash_cmd_launch())
        {
            return 2; //д���������
        }
        buf += 4;
    }

    return 0; //�ɹ�ִ��
}


uint32_t Flash_cmd_launch(void)
{
    // ������ʴ����־λ�ͷǷ����ʱ�־λ
    FTFL_FSTAT = (1 << 5) | (1 << 4);

    // ��������
    FTFL_FSTAT = (1 << 7);

    // �ȴ��������
    while (!(FTFL_FSTAT & (1 << 7)))
    {
        ;
    }

    // �������־
    if (FTFL_FSTAT & ((1 << 5) | (1 << 4) | 1))
    {
        return 1; //ִ���������
    }

    return 0; //ִ������ɹ�
}
