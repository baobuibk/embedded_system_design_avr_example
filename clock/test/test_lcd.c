/*
 * test_lcd.c
 *
 * Các hàm test hi?n th? LCD 16x2
 */

#include "test_lcd.h"
#include "lcd_16_2_4bit.h"
#include "bsp_lcd.h"
#include "bsp_timer.h"

#include <stdint.h>
#include <stdio.h>   /* dùng sprintf, n?u ng?i n?ng có th? ??i sang itoa */
#define LCD_TEST_PAUSE_INTERVAL 1000
/* Hàm ti?n l?i: dùng luôn delay c?a driver */
static void test_delay_ms(uint16_t ms)
{
    BSP_DelayMs(ms);
}

/* 1. Kh?i t?o và in thông ?i?p chào */
void TEST_LCD_InitAndHello(void)
{
    BSP_LCD_Init();   /* config PORTA */
    LCD_Init();       /* init LCD 4-bit */

    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_PutString(" LCD 16x2 TEST ");
    LCD_GotoXY(0, 1);
    LCD_PutString("Hello, world!  ");

    test_delay_ms(LCD_TEST_PAUSE_INTERVAL);
	
}

/* 2. Demo ??m s? trên dòng 2 */
void TEST_LCD_CounterDemo(uint16_t maxCount, uint16_t delayMs)
{
    char buf[17];

    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_PutString("Counter demo    ");

    for (uint16_t i = 0; i <= maxCount; i++) {
        /* format: "Count: xxxx     " */
        snprintf(buf, sizeof(buf), "Count: %-5u", i);

        LCD_GotoXY(0, 1);
        LCD_PutString("                ");  /* clear line */
        LCD_GotoXY(0, 1);
        LCD_PutString(buf);

		test_delay_ms(LCD_TEST_PAUSE_INTERVAL);
    }

        test_delay_ms(LCD_TEST_PAUSE_INTERVAL);

}

/* 3. Demo cu?n ch? trên dòng 1 */
void TEST_LCD_ScrollDemo(const char *line2Text, uint16_t scrollDelayMs)
{
    const char *msg = "Demo scroll text on line 1 - 4-bit LCD driver ";
    char window[17];
    uint16_t len = 0;

    /* Tính ?? dài chu?i msg */
    while (msg[len] != '\0') {
        len++;
    }

    LCD_Clear();

    /* In s?n dòng 2 */
    LCD_GotoXY(0, 1);
    if (line2Text != 0) {
        LCD_PutString(line2Text);
    }

    /* Cu?n t?ng "c?a s?" 16 ký t? */
    for (uint16_t offset = 0; offset < len; offset++) {
        for (uint8_t i = 0; i < 16; i++) {
            uint16_t idx = offset + i;
            if (idx < len) {
                window[i] = msg[idx];
            } else {
                window[i] = ' ';
            }
        }
        window[16] = '\0';

        LCD_GotoXY(0, 0);
        LCD_PutString(window);

        test_delay_ms(500);

    }

    test_delay_ms(LCD_TEST_PAUSE_INTERVAL);

}

/* 4. Demo b?t / t?t hi?n th? (blink nguyên màn hình) */
void TEST_LCD_BlinkDisplayDemo(uint8_t times, uint16_t delayMs)
{
    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_PutString("Blink display   ");
    LCD_GotoXY(0, 1);
    LCD_PutString("Display ON/OFF  ");

    for (uint8_t i = 0; i < times; i++) {
        /* Display OFF: 0x08 */
        LCD_SendCommand(0x08u);
        test_delay_ms(LCD_TEST_PAUSE_INTERVAL);


        /* Display ON, cursor off, blink off: 0x0C */
        LCD_SendCommand(0x0Cu);
		test_delay_ms(LCD_TEST_PAUSE_INTERVAL);
    }
}

/* 5. Run all: g?i l?n l??t các demo trên */
void TEST_LCD_RunAll(void)
{
    TEST_LCD_InitAndHello();
    TEST_LCD_CounterDemo(50, 100);               /* ??m t? 0..50, m?i 100ms */
    TEST_LCD_ScrollDemo("Scroll demo line2", 200);
    TEST_LCD_BlinkDisplayDemo(6, 200);
	LCD_Clear();
	LCD_GotoXY(0, 0);
	LCD_PutString("Tests finished  ");
	LCD_GotoXY(0, 1);
	LCD_PutString("Ready to use    ");

	while (1) {
		    /* main loop */
	    }
}
