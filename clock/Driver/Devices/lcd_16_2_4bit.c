/*
 * lcd_16_2_4bit.c
 *
 * Driver ?i?u khi?n LCD 16x2 dùng giao ti?p 4-bit (HD44780 compatible)
 * API chính ???c khai báo trong lcd_16_2_4bit.h
 *
 * Ph?n này ch? ch?a core logic.
 * Ph?n map chân GPIO + delay ???c cung c?p d??i d?ng hàm weak
 * ?? ng??i dùng override theo MCU c? th?.
 */

#include "lcd_16_2_4bit.h"
#include <stdint.h>

/*******************************************************************************
 *                    ??NH NGH?A MACRO WEAK PHÙ H?P COMPILER
 *******************************************************************************/

#ifndef LCD_WEAK
  #if defined(__GNUC__)
    #define LCD_WEAK __attribute__((weak))
  #elif defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION)
    #define LCD_WEAK __weak
  #else
    #define LCD_WEAK
    #warning "Weak attribute not supported on this compiler, overriding may not work as expected"
  #endif
#endif

/*******************************************************************************
 *      PH?N C?NG – HÀM WEAK, NG??I DÙNG CÓ TH? OVERRIDE ? FILE KHÁC
 *
 *******************************************************************************/

/* ?i?u khi?n chân RS, EN */
LCD_WEAK void LCD_RS_HIGH(void) { (void)0; }
LCD_WEAK void LCD_RS_LOW(void)  { (void)0; }
LCD_WEAK void LCD_EN_HIGH(void) { (void)0; }
LCD_WEAK void LCD_EN_LOW(void)  { (void)0; }

/* Ghi m?c logic cho 4 ???ng d? li?u D4..D7 */
LCD_WEAK void LCD_WriteBus4(uint8_t nibble)
{
	(void)nibble;   /* stub không làm gì, ?? BSP override */
}



LCD_WEAK void LCD_DelayMs(uint32_t ms)
{
    (void)ms;
}

LCD_WEAK void LCD_DelayUs(uint32_t us)
{
    (void)us;
}

/*******************************************************************************
 *                           PH?N CORE C?A LCD
 *******************************************************************************/

static void LCD_PulseEnable(void);
static void LCD_Write4Bits(uint8_t nibble);
static void LCD_Send(uint8_t value, uint8_t isData);

/* G?i xung Enable ?? LCD ch?t d? li?u */
static void LCD_PulseEnable(void)
{
	//PORTA |= (1<<2);
	//PORTA &= ~(1<<2);
    LCD_EN_HIGH();
    LCD_DelayUs(1);      /* t_pweh t?i thi?u ~450ns */
    LCD_EN_LOW();
    LCD_DelayUs(50);     /* Th?i gian x? lý l?nh ~37us tr? lên */
}

/* Ghi 4 bit (nibble) lên bus D4..D7
 * ? ?ây dùng 4 bit th?p c?a 'nibble' (bit0..3)
 */
static void LCD_Write4Bits(uint8_t nibble)
{
	LCD_WriteBus4(nibble & 0x0F);   /* ch? dùng 4 bit th?p */
	LCD_PulseEnable();
}

/* G?i 1 byte: tách thành 2 nibble 4-bit
 * isData = 0 ? l?nh (RS=0); isData = 1 ? d? li?u (RS=1)
 */
static void LCD_Send(uint8_t value, uint8_t isData)
{
    if (isData) {
        LCD_RS_HIGH();
    } else {
        LCD_RS_LOW();
    }

    /* G?i nibble cao (bits 7..4) */
    LCD_Write4Bits(value >> 4);
    /* G?i nibble th?p (bits 3..0) */
    LCD_Write4Bits(value & 0x0F);

    /* M?t s? l?nh ??c bi?t c?n delay lâu h?n */
    if (!isData && (value == 0x01u || value == 0x02u)) {
        /* Clear display ho?c Return home */
        LCD_DelayMs(2);
    }
}

/*******************************************************************************
 *                           CÁC HÀM PUBLIC API
 *******************************************************************************/

/* G?i l?nh ??n LCD */
void LCD_SendCommand(uint8_t cmd)
{
    LCD_Send(cmd, 0u);
}

/* G?i d? li?u (ký t?) ??n LCD */
void LCD_SendData(uint8_t data)
{
    LCD_Send(data, 1u);
}

/* Kh?i t?o LCD ? ch? ?? 4-bit, 2 dòng, font 5x8 */
void LCD_Init(void)
{
    /* Ch? LCD ?n ??nh sau khi c?p ngu?n (theo datasheet ~>15ms) */
    LCD_DelayMs(40);

    LCD_RS_LOW();
    LCD_EN_LOW();

    /* Trình t? kh?i t?o 4-bit theo chu?n HD44780:
       - G?i 0x03 (d??i d?ng 4-bit) 3 l?n ?? ép v? 8-bit mode.
       - Sau ?ó g?i 0x02 ?? chuy?n sang 4-bit mode.
       ? giai ?o?n này LCD "?ang ngh?" là ?ang nh?n bus 8-bit,
       nên ta ch? g?i nibble cao (4 bit) c?a pattern.
    */

    LCD_Write4Bits(0x03u);
    LCD_DelayMs(5);

    LCD_Write4Bits(0x03u);
    LCD_DelayUs(150);

    LCD_Write4Bits(0x03u);
    LCD_DelayUs(150);

    /* Chuy?n sang 4-bit mode */
    LCD_Write4Bits(0x02u);
    LCD_DelayUs(150);

    /* Function Set: 4-bit, 2 dòng, font 5x8
       0x28 = 0b0010 1000
    */
    LCD_SendCommand(0x28u);

    /* Display OFF: t?t hi?n th?, t?t cursor, t?t blink */
    LCD_SendCommand(0x08u);

    /* Clear display */
    LCD_Clear();

    /* Entry Mode Set: t?ng ??a ch? DDRAM, không d?ch màn hình
       0x06 = 0b0000 0110
    */
    LCD_SendCommand(0x06u);

    /* Display ON: b?t hi?n th?, t?t cursor, t?t blink
       0x0C = 0b0000 1100
    */
    LCD_SendCommand(0x0Cu);
}

/* Xóa màn hình LCD */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01u);   /* Clear display */
}

/* ??a con tr? v? v? trí home (0,0) */
void LCD_Home(void)
{
    LCD_SendCommand(0x02u);   /* Return home */
}

/* Ghi 1 ký t? lên v? trí hi?n t?i c?a con tr? */
void LCD_PutChar(char c)
{
    LCD_SendData((uint8_t)c);
}

/* Ghi chu?i null-terminated */
void LCD_PutString(const char *s)
{
    if (s == 0) return;

    while (*s != '\0') {
        LCD_PutChar(*s++);
    }
}

/* ??a con tr? t?i (col, row)
 * row: 0 ho?c 1
 * col: 0..15
 */
void LCD_GotoXY(uint8_t col, uint8_t row)
{
    uint8_t addr;

    if (col > 15u) {
        col = 15u;
    }

    switch (row) {
    case 0u:
        addr = 0x00u + col;
        break;
    case 1u:
    default:
        addr = 0x40u + col;   /* Dòng 2 b?t ??u t?i DDRAM 0x40 */
        break;
    }

    LCD_SendCommand(0x80u | addr);  /* Set DDRAM address */
}
