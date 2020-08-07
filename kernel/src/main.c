#ifndef STM32F411xE
    #define STM32F411xE
#endif
#include "stm32f4xx.h"

void TIM2_IRQHandler(void) {
  // flash on update event
  if (TIM2->SR & TIM_SR_UIF) GPIOD->ODR ^= (1 << 13);

  TIM2->SR = 0x0; // reset the status register
}

int main()
{
    // Initialize Clock for Port C
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;       // GPIOC Enable
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;       // Enable TIM2 clock

    GPIOC->MODER |= (1 << 26);                 // Ouput mode

    NVIC->ISER[0] |= 1<< (TIM2_IRQn); // enable the TIM2 IRQ

    TIM2->PSC = 0x0; // no prescaler, timer counts up in sync with the peripheral clock
    TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
    TIM2->ARR = 0x01; // count to 1 (autoreload value 1)
    TIM2->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN; // autoreload on, counter enabled
    TIM2->EGR = 1; // trigger update event to reload timer registers

    while (1) GPIOD->ODR ^= (1 << 13);

    return 0;
}
