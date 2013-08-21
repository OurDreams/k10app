/*
 * led.h
 *
 *  Created on: 2013-8-21
 *      Author: daixuyi
 */

#ifndef LED_H_
#define LED_H_

enum
{
	led_on =0,
	led_off,
	led_turn
};

extern status_t led_device_init(void);

#endif /* LED_H_ */
