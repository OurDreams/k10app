/**
 ******************************************************************************
 * @file       lcd_wr.h
 * @version    V1.1.4
 * @brief      API C source file of lcd_wr.h
 *             Created on: 2013-11-2
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
 ******************************************************************************
 */

#ifndef LCD_WR_H_
#define LCD_WR_H_

extern void LCD_DATA_OUT(uint8_t data);
extern void LCD_CMD_OUT(uint8_t data);
extern char* LCD_GETBASE();
extern void LCD_PowerOn();


#endif /* LCD_WR_H_ */
