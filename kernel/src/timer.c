#include "timer.h"
#include "gpio.h"

/*
 *  @brief Starts systick for ~1 second
 */
void SysTick_Start()
{
    gpio_init(GPIOC, 13, MODE_GP_OUTPUT, OUTPUT_PUSH_PULL, OUTPUT_SPEED_VERY_HIGH, PUPD_NONE, ALT0);

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
    gpio_toggle(GPIOC, 13);
}
