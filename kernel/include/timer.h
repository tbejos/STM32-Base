#ifndef _TIMER_H_
#define _TIMER_H

#include "stm32f4xx.h"


void SysTick_Start(void);
void SysTick_Stop(void);
void SysTick_Handler(void);

#endif
