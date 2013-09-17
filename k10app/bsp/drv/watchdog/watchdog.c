/**
 ******************************************************************************
 * @file       wdt.c
 * @version    V1.1.4
 * @brief      API C source file of watchdog.c
 *             Created on: 2013-9-17
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <types.h>
#include <gpio.h>
#include <devLib.h>
#include <led.h>
/**
 ******************************************************************************
 * @brief      �豸��ʼ��
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t dev_wdg_init(struct device* dev)
{
    /*��ʼ��PORTE5���1*/
    gpio_init(PORTA, 4, 1, 1);
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
static status_t dev_wdg_release(struct device* dev)
{
    printf("wdg release\n");
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
static status_t  dev_wdg_open(struct device* dev)
{
//  printf("gpio open\n");
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
static status_t  dev_wdg_close(struct device* dev)
{
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
static size_t dev_wdg_read (struct device* dev, int32_t pos, void *buffer, size_t pinno)
{

    return OK;
}

/**
 ******************************************************************************
 * @brief      GPIO�Ŀ���
 * @param[in]  device* dev : �豸����
 * @param[in]  cmd         ��0���õͣ�1���ø�
 * @param[in]  *arg        ��io����
 * @retval     OK          : �ɹ�
 * @retval     FALSE       : ʧ��
 ******************************************************************************
 */
static int32_t dev_wdg_ioctl (struct device* dev, uint32_t cmd, void* arg)
{
    if(cmd == 1)
    {
        gpio_turn(PORTA, 4);
//        printf("feed dog\n");
    }
    return OK;
}

const static fileopt_t wdg_opt =
{
        .init = dev_wdg_init,
        .release = dev_wdg_release,
        .open = dev_wdg_open,
        .read = dev_wdg_read,
        .close = dev_wdg_close,
        .ioctl = dev_wdg_ioctl,
};

status_t wdg_device_init(void)
{
    if (OK != dev_create("wdg", &wdg_opt, MKDEV(1, 1), NULL))
    {
        printf("wdg dev_create err\n");
        return ERROR;
    }
    return OK;
}
