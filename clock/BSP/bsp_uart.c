/*
 * bsp_uart.c
 *
 * Created: 11/20/2025 11:53:33 PM
 *  Author: Admin
 */ 

#include "uart.h"
#include "bsp_rtc.h"
#include "board.h"

void bsp_uart_init(void)
{
	UART_init(9600, CPU_FREQ);
}

void _putchar(char character)
{
	UART_sendByte(character);
}