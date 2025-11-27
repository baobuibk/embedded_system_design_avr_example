/*
 * bsp_timer.c
 */
#include "board.h"
#include "bsp_timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t sys_time_count = 0u;
extern uint16_t	app_button_timer_count;

void BSP_Timer1_Init(void)
{
    uint8_t sreg = SREG;
    cli();

    sys_time_count = 0u;


    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCNT1  = 0x0000;

    /* CTC mode: WGM12 = 1, WGM13:0 = 0b0100 */
    TCCR1B |= (1u << WGM12);  /* TCCR1A v?n = 0 */

 
 
    OCR1A = (uint16_t)(F_CPU / 64UL / 1000UL - 1UL);

    /* Enable Compare Match A interrupt cho Timer1 */
#if defined(TIMSK1)
    TIMSK1 |= (1u << OCIE1A);
#else

    TIMSK  |= (1u << OCIE1A);
#endif

    /* Start Timer1 prescaler = 64 */
    TCCR1B |= (1u << CS11) | (1u << CS10);

 
    SREG = sreg;
}

/* ISR Compare Match A  */
ISR(TIMER1_COMPA_vect)
{
    sys_time_count++;
	if (0 < app_button_timer_count) app_button_timer_count--;
}

/* Hàm ??c giá tr? ms m?t cách “atomic” (tránh ??c d? dang 32-bit) */
uint32_t BSP_GetSysTimeMs(void)
{
    uint32_t t;
   uint8_t sreg = SREG;
    cli();
    t = sys_time_count;
   SREG = sreg;

    return t;
}
/* Blocking delay using sys_time_count (Timer1 must be running, interrupts enabled) */
void BSP_DelayMs(uint32_t delayMs)
{
	uint32_t start = BSP_GetSysTimeMs();

	/* Dùng phép tr? ?? ch?u ???c overflow 32-bit */
	while ((uint32_t)(BSP_GetSysTimeMs() - start) < delayMs) {
	
	}
}