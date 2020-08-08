// For VS Code Linting Make sure to change
#ifndef _TIMER_H_
#define _TIMER_H
#ifndef STM32F411xE
    #define STM32F411xE
#endif
#include "stm32f4xx.h"

void SysTick_Start(void);
void SysTick_Stop(void);
void SysTick_Handler(void);

#endif
