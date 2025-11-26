#ifndef TEST_LCD_H_
#define TEST_LCD_H_

#include <stdint.h>

/* Kh?i t?o LCD (g?i BSP + LCD_Init) và in thông ?i?p chào */
void TEST_LCD_InitAndHello(void);

/* ??m s? trên dòng 2: 0 ? max, dùng ?? test refresh */
void TEST_LCD_CounterDemo(uint16_t maxCount, uint16_t delayMs);

/* Cu?n m?t câu dài trên dòng 1, dòng 2 hi?n th? c? ??nh */
void TEST_LCD_ScrollDemo(const char *line2Text, uint16_t scrollDelayMs);

/* Demo b?t / t?t hi?n th? (blink effect) */
void TEST_LCD_BlinkDisplayDemo(uint8_t times, uint16_t delayMs);

/* Ch?y t?t c? demo l?n l??t (tùy b?n dùng ho?c không) */
void TEST_LCD_RunAll(void);

#endif /* TEST_LCD_H_ */
