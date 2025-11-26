#ifndef BSP_LCD_H_
#define BSP_LCD_H_

#include <stdint.h>

/* Kh?i t?o GPIO cho LCD:
 * PA0 ? RS
 * PA1 ? RW
 * PA2 ? E
 * PA4 ? D4
 * PA5 ? D5
 * PA6 ? D6
 * PA7 ? D7
 */
void BSP_LCD_Init(void);
void LCD_DelayUs(uint32_t us);
void LCD_DelayMs(uint32_t ms);

#endif /* BSP_LCD_H_ */
