#ifndef LCD_16_2_4BIT_H_
#define LCD_16_2_4BIT_H_

#include <stdint.h>

/* API c? b?n */
void LCD_Init(void);
void LCD_Clear(void);
void LCD_Home(void);

void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);

void LCD_PutChar(char c);
void LCD_PutString(const char *s);
void LCD_GotoXY(uint8_t col, uint8_t row);

/* row: 0 ho?c 1
 * col: 0..15
 */

#endif /* LCD_16_2_4BIT_H_ */
