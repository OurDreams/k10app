/*
 * led.c
 *
 *  Created on: 2013-8-21
 *      Author: daixuyi
 */
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
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
static status_t dev_led_init(struct device* dev)
{
	/*��ʼ��PORTE5���1*/
	gpio_init(PORTE, 6, 1, 1);
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
static status_t dev_led_release(struct device* dev)
{
	printf("gpio release\n");
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
static status_t  dev_led_open(struct device* dev)
{
	printf("gpio open\n");
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
static status_t  dev_led_close(struct device* dev)
{
	printf("gpio close\n");
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
static size_t dev_led_read (struct device* dev, int32_t pos, void *buffer, size_t pinno)
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
static int32_t dev_led_ioctl (struct device* dev, uint32_t cmd, void* arg)
{
	if(cmd == led_on)
	{
		gpio_set(PORTE, 6, 1);
	}
	else if( cmd == led_off)
	{
		gpio_set(PORTE, 6, 0);
	}
	else if(cmd == led_turn)
	{
		gpio_turn(PORTE, 6);
	}
	return OK;
}

const static fileopt_t gpio_opt =
{
		.init = dev_led_init,
		.release = dev_led_release,
		.open = dev_led_open,
		.read = dev_led_read,
		.close = dev_led_close,
		.ioctl = dev_led_ioctl,
};

status_t led_device_init(void)
{
	if (OK != dev_create("leds", &gpio_opt, MKDEV(1, 0), NULL))
    {
	    printf("dev_create err\n");
	    return ERROR;
    }
	return OK;
}

