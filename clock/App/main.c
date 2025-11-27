/*
 * main.c
 *
 * Created: 11/20/2025 9:30:04 PM
 *  Author: Admin
 */ 

#include <xc.h>
#include "ds1307.h"
#include "bsp_rtc.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "uart.h"
#include "printf.h"
#include "test_rtc.h"
#include "test_lcd.h"
#include "bsp_timer.h"
#include "bsp_button.h"
#include "app_button.h"
#include "lcd_16_2_4bit.h"
#include <avr/interrupt.h>
int main(void)
{

		DS1307_DateTimeType pre;
		DS1307_DateTimeType now;
		DS1307_DateTimeType t = {
			.seconds = 0,
			.minutes = 30,
			.hours   = 21,
			.day     = 4,   
			.date    = 20,
			.month   = 11,
			.year    = 25   
		};
	bsp_rtc_init(&t);
	bsp_uart_init();
	bsp_rtc_get_date_time(&pre);
	bsp_button_init();
	BSP_Timer1_Init();
	sei();
	
	BSP_LCD_Init();
	LCD_Init();       /* init LCD 4-bit */

	LCD_Clear();
//	BSP_DelayMs(100);
//	test_rtc_main();
//	TEST_LCD_RunAll();

	while (1)
	{
		app_button_update();
		if (BUTTON_PRESSED == app_button_get_state())
		{
			 LCD_GotoXY(0, 0);
			 LCD_PutString(" BUTTON PRESSED   ");
		}
		else
		{
			 LCD_GotoXY(0, 0);
			 LCD_PutString(" BUTTON RELEASED  ");
		}
		bsp_rtc_get_date_time(&now);
		if (now.seconds != pre.seconds)	
		{
				printf("h: %d m: %d s: %d \r\n", now.hours, now.minutes, now.seconds);
				pre = now;			
		}
	}

}