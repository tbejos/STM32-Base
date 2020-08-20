#include "timer.h"
#include "gpio.h"

/*
 *  @brief Starts systick for ~1 second
 */
void SysTick_Start()
{
    GPIO_InitStruct init = {0};
    init.pin = 13;
    init.mode = MODE_OUTPUT_PP;
    init.pupd = PUPD_NONE;
    init.speed = OUTPUT_SPEED_VERY_HIGH;

    gpio_init(GPIOC, &init);

    SysTick->LOAD = 12000000;
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
