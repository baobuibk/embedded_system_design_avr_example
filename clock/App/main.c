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
#include "uart.h"
#include "printf.h"
#include "test_rtc.h"
#include "test_lcd.h"
int main(void)
{

//	test_rtc_main();
	TEST_LCD_RunAll();

}