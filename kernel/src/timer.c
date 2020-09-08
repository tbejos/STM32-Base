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

    // For 1s   (uint32_t)(SystemCoreClock - 1U);
    // For 1ms  (uint32_t)((SystemCoreClock / 1000) - 1U);
    SysTick->LOAD = (uint32_t)((SystemCoreClock / 1000) - 1U);
    SysTick->VAL = 0U;
    SysTick->CTRL = (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
    return;
}

void SysTick_Stop()
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

static uint64_t milliseconds;

uint64_t millis()
{
    return milliseconds;
}

void SysTick_Handler(void)
{
    milliseconds++;
    if (milliseconds % 1000 == 0)
        gpio_toggle(GPIOC, 13);
}
