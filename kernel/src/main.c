#ifndef STM32F411xE
    #define STM32F411xE
#endif
#include "stm32f4xx.h"
#include "timer.h"

int main()
{
    SysTick_Start();

    while (1);

    return 0;
}
