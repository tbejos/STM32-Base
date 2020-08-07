#include "timer.h"

/*
 *  @brief Starts systick for ~1 second
 */
void SysTick_Start()
{
        // Initialize Clock for Port C
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;        // GPIOC Enable

    GPIOC->MODER |= (1 << 26);                  // Ouput mode

    SysTick->LOAD = 1250000;
    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
    return;
}

void SysTick_Stop()
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    GPIOC->ODR ^= (1 << 13);
}
