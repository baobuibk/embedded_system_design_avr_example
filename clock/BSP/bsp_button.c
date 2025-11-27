/*
 * bsp_button.c
 *
 * Created: 11/27/2025 11:10:25 PM
 *  Author: Admin
 */ 
#include "bsp_button.h"
#include <avr/io.h>


uint8_t bsp_button_start_get_state(void){
	/* PC0 ?ang dùng làm input cho nút nh?n */
	if ((PINC & (1 << PC7)) == 0)
	{
		/* ??c ???c m?c 0 ? nút ?ang ???c nh?n */
		return BUTTON_PRESSED;
	}
	else
	{
		/* ??c ???c m?c 1 ? nút ?ang th? */
		return BUTTON_RELEASED;
	}
}
void bsp_button_init(void)
{
	/* PC0 là input */
	DDRC &= ~(1 << PC7);

	/* B?t ?i?n tr? kéo lên (pull-up) cho PC0 */
	PORTC |= (1 << PC7);
}