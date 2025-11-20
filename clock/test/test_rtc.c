/*
 * test_rtc.c
 *
 * Created: 11/21/2025 12:15:03 AM
 *  Author: Admin
 */ 

#include "ds1307.h"
#include "bsp_rtc.h"
#include "bsp_uart.h"
#include "uart.h"
#include "printf.h"
#include "test_rtc.h"
int test_rtc_main(void)
{

	DS1307_DateTimeType pre;
	DS1307_DateTimeType now;
	DS1307_DateTimeType t = {
		.seconds = 0,
		.minutes = 30,
		.hours   = 21,
		.day     = 4,   /* ví d?: 4 = Thursday */
		.date    = 20,
		.month   = 11,
		.year    = 25   /* 2025 */
	};
	bsp_rtc_init(&t);
	bsp_uart_init();
	bsp_rtc_get_date_time(&pre);
	while (1)
	{

		bsp_rtc_get_date_time(&now);
		if (now.seconds == pre.seconds)	continue;
		printf("h: %d m: %d s: %d \r\n", now.hours, now.minutes, now.seconds);
		pre = now;
	}
}