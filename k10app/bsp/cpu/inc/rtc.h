/*
 * File:		rtc.h
 * Purpose:     Provide common rtc functions
 *
 * Notes:
 */

#ifndef __RTC_H__
#define __RTC_H__

/********************************************************************/

void rtc_init(uint32_t seconds, uint32_t alarm, uint8_t c_interval, uint8_t c_value, uint8_t interrupt);
void rtc_isr(void);

/********************************************************************/

#endif /* __RTC_H__ */
