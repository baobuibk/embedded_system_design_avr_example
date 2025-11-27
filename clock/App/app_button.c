/*
 * app_button.c
 *
 * Created: 11/27/2025 10:46:36 PM
 *  Author: Admin
 */ 

#include <stdio.h>
#include "bsp_button.h"
typedef enum {S_BUTTON_PRESSED, S_BUTTON_PRESSED_CHECK, S_BUTTON_RELEASED, S_BUTTON_RELEASED_CHECK} button_state_t;
	
static button_state_t state = S_BUTTON_RELEASED;

uint16_t	app_button_timer_count;

void app_button_update()
{
	switch (state)
	{
		case S_BUTTON_RELEASED:
			if (BUTTON_PRESSED == bsp_button_start_get_state())
			{
				state = S_BUTTON_PRESSED_CHECK;
				app_button_timer_count = 2000;	//wait for 2000 ms
			}
			break;
		case S_BUTTON_PRESSED_CHECK:
			if (BUTTON_RELEASED == bsp_button_start_get_state())
			{
				state = S_BUTTON_RELEASED;
				app_button_timer_count = 0;	//stop timer
			}
			else if (0 == app_button_timer_count)	//timer fired
			{
				state = S_BUTTON_PRESSED;				
			}
			break;		
		case S_BUTTON_PRESSED:
			if (BUTTON_RELEASED == bsp_button_start_get_state())
			{
				state = S_BUTTON_RELEASED_CHECK;
				app_button_timer_count = 2000;	//wait for 2000 ms
			}
			break;
		case S_BUTTON_RELEASED_CHECK:
			if (BUTTON_PRESSED == bsp_button_start_get_state())
			{
				state = S_BUTTON_PRESSED;
				app_button_timer_count = 0;	//stop timer
			}
			else if (0 == app_button_timer_count)	//timer fired
			{
				state = S_BUTTON_RELEASED;
			}
			break;
		default:
			break;
	}
}

uint8_t app_button_get_state()
{
	switch (state)
	{
		case S_BUTTON_RELEASED:
		case S_BUTTON_PRESSED_CHECK:
			return BUTTON_RELEASED;
		case S_BUTTON_PRESSED:
		case S_BUTTON_RELEASED_CHECK:
			return BUTTON_PRESSED;
		default:
			return BUTTON_RELEASED;
	}
	
}