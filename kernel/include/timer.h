#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"

void SysTick_Start(void);
void SysTick_Stop(void);
void SysTick_Handler(void);
uint64_t millis();

#endif /* _TIMER_H_ */
