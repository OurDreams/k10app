/**
 ******************************************************************************
 * @file       yx_drv.c
 * @version    V1.1.4
 * @brief      API C source file of yx_drv.c
 *             Created on: 2013-11-7
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
    uint8_t cs0_yx_val, cs1_yx_val, cs2_yx_val, cs4_yx_val, cs5_yx_val;
//    uint8_t buff[4];
    uint8_t *buff = buffer;
    cs0_yx_val = *CS0_START_ADDRESS;
    cs1_yx_val = *CS1_START_ADDRESS;
    cs2_yx_val = *CS2_START_ADDRESS;
    cs4_yx_val = *CS4_START_ADDRESS;
    cs5_yx_val = *CS5_START_ADDRESS;

    buff[0] = cs0_yx_val;
    buff[1] = cs1_yx_val;
    buff[2] = cs2_yx_val;
    buff[3] = cs4_yx_val;
    buff[4] = cs5_yx_val;

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
    return OK;
}

const static fileopt_t yx_opt =
{
        .init = dev_relay_init,
        .release = dev_relay_release,
        .open = dev_relay_open,
        .read = dev_relay_read,
        .close = dev_relay_close,
        .ioctl = dev_relay_ioctl,
};

status_t yx_device_init(void)
{
    if (OK != dev_create("yx", &yx_opt, MKDEV(3, 0), NULL))
    {
        printf("dev_create err\n");
        return ERROR;
    }
    return OK;
}
