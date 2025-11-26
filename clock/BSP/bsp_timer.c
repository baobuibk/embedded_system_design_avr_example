/*
 * bsp_timer.c
 *
 * Kh?i t?o Timer1 ? ch? ?? CTC, ng?t m?i 1 ms.
 * Trong ISR: t?ng sys_time_count lên 1.
 */
#include "board.h"
#include "bsp_timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t sys_time_count = 0u;

/* Kh?i t?o Timer1:
 * - Mode: CTC (Clear Timer on Compare Match)
 * - Prescaler: 64
 * - OCR1A: ???c tính ?? ra 1 ms
 */
void BSP_Timer1_Init(void)
{
    /* T?m th?i disable global interrupt trong lúc c?u hình */
    uint8_t sreg = SREG;
    cli();

    sys_time_count = 0u;

    /* D?ng Timer1, clear c?u hình */
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCNT1  = 0x0000;

    /* CTC mode: WGM12 = 1, WGM13:0 = 0b0100 */
    TCCR1B |= (1u << WGM12);  /* TCCR1A v?n = 0 */

    /* Ch?n prescaler = 64: CS11 = 1, CS10 = 1 */
    /* OCR1A tính theo F_CPU và prescaler 64:
     *  t = (OCR1A + 1) * prescaler / F_CPU
     *  mu?n t = 1ms => OCR1A = F_CPU/64/1000 - 1
     */
    OCR1A = (uint16_t)(F_CPU / 64UL / 1000UL - 1UL);

    /* Enable Compare Match A interrupt cho Timer1 */
#if defined(TIMSK1)
    TIMSK1 |= (1u << OCIE1A);
#else
    /* M?t s? AVR c? dùng TIMSK */
    TIMSK  |= (1u << OCIE1A);
#endif

    /* Start Timer1 v?i prescaler = 64 */
    TCCR1B |= (1u << CS11) | (1u << CS10);

    /* Khôi ph?c tr?ng thái interrupt tr??c ?ó */
    SREG = sreg;
}

/* ISR Compare Match A c?a Timer1: ch?y m?i 1 ms */
ISR(TIMER1_COMPA_vect)
{
    sys_time_count++;
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
		/* có th? thêm asm("nop"); n?u mu?n */
	}
}