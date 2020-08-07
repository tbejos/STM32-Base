#ifndef STM32F411xE
    #define STM32F411xE
#endif
#include "stm32f4xx.h"

void SysTick_Handler(void)
{
    GPIOC->ODR ^= (1 << 13);
}

int main()
{
    // Initialize Clock for Port C
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;        // GPIOC Enable

    GPIOC->MODER |= (1 << 26);                  // Ouput mode

    SysTick->LOAD = 1250000;
    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

    while (1);

    return 0;
}
