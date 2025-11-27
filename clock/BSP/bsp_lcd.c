/*
 * bsp_lcd.c
 *
 * PA0 ? RS
 * PA1 ? RW
 * PA2 ? E
 * PA4 ? D4
 * PA5 ? D5
 * PA6 ? D6
 * PA7 ? D7
 */

#include "lcd_16_2_4bit.h"
#include "bsp_lcd.h"
#include "board.h"
#include <avr/io.h>
#include <util/delay.h>

static inline void bsp_lcd_set_bit(volatile uint8_t *port, uint8_t bit)
{
    *port |= (1u << bit);
}

static inline void bsp_lcd_clear_bit(volatile uint8_t *port, uint8_t bit)
{
    *port &= ~(1u << bit);
}

static inline void bsp_lcd_force_write_mode(void)
{
    /* RW = 0: luôn ghi, không ??c tr?ng thái t? LCD */
    bsp_lcd_clear_bit(&PORTA, PA1);
}

/*******************************************************************************
 *                             HÀM PUBLIC
 *******************************************************************************/

void BSP_LCD_Init(void)
{
    /* PA0, PA1, PA2, PA4..PA7 là output */
    DDRA |= (1u << PA0) | (1u << PA1) | (1u << PA2) |
            (1u << PA4) | (1u << PA5) | (1u << PA6) | (1u << PA7);

    /* M?c ??nh kéo v? 0 */
    bsp_lcd_clear_bit(&PORTA, PA0);
    bsp_lcd_clear_bit(&PORTA, PA1);
    bsp_lcd_clear_bit(&PORTA, PA2);
    bsp_lcd_clear_bit(&PORTA, PA4);
    bsp_lcd_clear_bit(&PORTA, PA5);
    bsp_lcd_clear_bit(&PORTA, PA6);
    bsp_lcd_clear_bit(&PORTA, PA7);
}

/*******************************************************************************
 *                   OVERRIDE CÁC HÀM WEAK C?A LCD DRIVER
 *******************************************************************************/

/* RS: PA0 */
void LCD_RS_HIGH(void)
{
    bsp_lcd_force_write_mode();
    bsp_lcd_set_bit(&PORTA, PA0);
}

void LCD_RS_LOW(void)
{
    bsp_lcd_force_write_mode();
    bsp_lcd_clear_bit(&PORTA, PA0);
}

/* EN: PA2 */
void LCD_EN_HIGH(void)  
{
    bsp_lcd_force_write_mode();
    bsp_lcd_set_bit(&PORTA, PA2);
}

void LCD_EN_LOW(void)
{
    bsp_lcd_force_write_mode();
    bsp_lcd_clear_bit(&PORTA, PA2);
}

#define LCD_DATA_MASK   ((1u << PA4) | (1u << PA5) | (1u << PA6) | (1u << PA7))

void LCD_WriteBus4(uint8_t nibble)
{
	bsp_lcd_force_write_mode();

	/* Gi? nguyên các bit khác, ch? thay 4 bit PA4..PA7 b?ng (nibble & 0x0F) */
	PORTA = (uint8_t)((PORTA & ~LCD_DATA_MASK) | ((nibble & 0x0Fu) << 4));
}

/*******************************************************************************
 *                              DELAY
 *******************************************************************************/

void LCD_DelayMs(uint32_t ms)
{
    while (ms--) {
		LCD_DelayUs(1000);
    }
}

void LCD_DelayUs(uint32_t us)
{
    while (us--) {
		for (uint8_t i = 100; i>0; i--);
    }
}
