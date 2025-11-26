#ifndef BSP_TIMER_H_
#define BSP_TIMER_H_

#include <stdint.h>

/* Bi?n ??m th?i gian h? th?ng, ??n v?: ms */
extern volatile uint32_t sys_time_count;

/* Kh?i t?o Timer1 ? ch? ?? CTC, t?o ng?t m?i 1 ms */
void BSP_Timer1_Init(void);

uint32_t BSP_GetSysTimeMs(void);
/* Blocking delay in ms using Timer1/sys_time_count */
void BSP_DelayMs(uint32_t delayMs);
#endif /* BSP_TIMER_H_ */
