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
 * @brief      设备初始化
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t dev_led_init(struct device* dev)
{
	/*初始化PORTE5输出1*/
	gpio_init(PORTE, 6, 1, 1);
	return OK;
}
/**
 ******************************************************************************
 * @brief      设备释放
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
 * @brief      打开设备
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
 * @brief      关闭设备
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
 * @brief      读GPIO
 * @param[in]  device* dev : 设备描述
 * @param[in]  *buffer     : 写缓存地址
 * @param[in]  pinno       : 读取的引脚

 * @retval     OK          : 成功
 * @retval     FALSE       : 失败
 ******************************************************************************
 */
static size_t dev_led_read (struct device* dev, int32_t pos, void *buffer, size_t pinno)
{

	return OK;
}

/**
 ******************************************************************************
 * @brief      GPIO的控制
 * @param[in]  device* dev : 设备描述
 * @param[in]  cmd         ：0，置低；1，置高
 * @param[in]  *arg        ：io引脚
 * @retval     OK          : 成功
 * @retval     FALSE       : 失败
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

