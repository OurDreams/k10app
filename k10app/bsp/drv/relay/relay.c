/**
 ******************************************************************************
 * @file       relay.c
 * @version    V1.1.4
 * @brief      API C source file of relay.c
 *             Created on: 2013-9-9
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
 ******************************************************************************
 */

#include <relay.h>
#include <flexbus.h>
#include <stdio.h>
#include <types.h>
#include <devLib.h>

/*��բִ��*/
void close_action()
{
    *CS0_START_ADDRESS &= 0xf8;
//    printf("��բִ��\n");
}
/*��բִ��*/
void open_action()
{
    *CS0_START_ADDRESS &= 0xf4;
//    printf("��բִ��\n");
}

typedef struct
{
    uint8_t relay_num;  //
    uint8_t *paddr;     //
    uint8_t open_val;   //
    uint8_t close_val;  //
//    void (*p_func)(void);
}relay_status_t;

static relay_status_t yaokong[] =
{
        {relay_o1, CS0_START_ADDRESS, 0xef, 0xff},
        {relay_o2, CS0_START_ADDRESS, 0xbf, 0xff},
        {relay_o3, CS1_START_ADDRESS, 0xfe, 0xff},
        {relay_o4, CS1_START_ADDRESS, 0xfb, 0xff},
        {relay_o5, CS1_START_ADDRESS, 0xef, 0xff},
        {relay_o6, CS1_START_ADDRESS, 0xbf, 0xff},
        {relay_o7, CS2_START_ADDRESS, 0xfe, 0xff},
        {relay_o8, CS2_START_ADDRESS, 0xfb, 0xff},
        {relay_o9, CS2_START_ADDRESS, 0xef, 0xff},
        {relay_o10, CS2_START_ADDRESS, 0xbf, 0xff},

        {relay_c1, CS0_START_ADDRESS, 0xdf, 0xff},
        {relay_c2, CS0_START_ADDRESS, 0x7f, 0xff},
        {relay_c3, CS1_START_ADDRESS, 0xfd, 0xff},
        {relay_c4, CS1_START_ADDRESS, 0xf7, 0xff},
        {relay_c5, CS1_START_ADDRESS, 0xdf, 0xff},
        {relay_c6, CS1_START_ADDRESS, 0x7f, 0xff},
        {relay_c7, CS2_START_ADDRESS, 0xfd, 0xff},
        {relay_c8, CS2_START_ADDRESS, 0xf7, 0xff},
        {relay_c9, CS2_START_ADDRESS, 0xdf, 0xff},
        {relay_c10, CS2_START_ADDRESS, 0x7f, 0xff},

};

/**
 ******************************************************************************
 * @brief      �豸��ʼ��
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t dev_relay_init(struct device* dev)
{
    return OK;
}
/**
 ******************************************************************************
 * @brief      �豸�ͷ�
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t dev_relay_release(struct device* dev)
{
//    printf("relay device release\n");
    return OK;
}
/**
 ******************************************************************************
 * @brief      ���豸
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t  dev_relay_open(struct device* dev)
{
//    printf("relay device open\n");
    return OK;
}
/**
 ******************************************************************************
 * @brief      �ر��豸
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t  dev_relay_close(struct device* dev)
{
//    printf("relay device close\n");
    return OK;
}
/**
 ******************************************************************************
 * @brief      ��GPIO
 * @param[in]  device* dev : �豸����
 * @param[in]  *buffer     : д�����ַ
 * @param[in]  pinno       : ��ȡ������

 * @retval     OK          : �ɹ�
 * @retval     FALSE       : ʧ��
 ******************************************************************************
 */
static size_t dev_relay_read (struct device* dev, int32_t pos, void *buffer, size_t pinno)
{

    return OK;
}

/**
 ******************************************************************************
 * @brief
 * @param[in]  device* dev : �豸����
 * @param[in]  cmd         ��0���õͣ�1���ø�
 * @param[in]  *arg        ��
 * @retval     OK          : �ɹ�
 * @retval     FALSE       : ʧ��
 ******************************************************************************
 */
static int32_t dev_relay_ioctl (struct device* dev, uint32_t cmd, void* arg)
{
    uint8_t i;
    for(i = 0; i < ARRAY_SIZE(yaokong); i++)
    {
        if(yaokong[i].relay_num == ((uint8_t)(*((uint8_t*)arg))))
        {
            if(cmd == 1u)
            {
                *(yaokong[i].paddr) &= yaokong[i].close_val;
                if(yaokong[i].relay_num < 10)
                {
                    open_action();
                }
                else
                {
                    close_action();
                }
            }
            else if(cmd == 0u)
            {
                *(yaokong[i].paddr) &= yaokong[i].open_val;
                printf("0x%x = 0x%x\n", yaokong[i].paddr, *(yaokong[i].paddr));/*û�еĻ���1~4�ż̵���������why��*/

                if(yaokong[i].relay_num < 10)
                {
                    open_action();
                }
                else
                {
                    close_action();
                }
            }
            else
            {
                printf("Wrong command %d!\n\r", cmd);
            }
        }
    }
    return OK;
}

const static fileopt_t relay_opt =
{
        .init = dev_relay_init,
        .release = dev_relay_release,
        .open = dev_relay_open,
        .read = dev_relay_read,
        .close = dev_relay_close,
        .ioctl = dev_relay_ioctl,
};

status_t relay_device_init(void)
{
    if (OK != dev_create("relays", &relay_opt, MKDEV(2, 0), NULL))
    {
        printf("dev_create err\n");
        return ERROR;
    }
    return OK;
}

