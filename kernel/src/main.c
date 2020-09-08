#include "stm32f4xx.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "printk.h"

#define RCC_PLLCFGR_PLLM_DIV25  (RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_0)
#define RCC_PLLCFGR_PLLN_x192   (RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLN_6)
#define RCC_PLLCFGR_PLLQ_DIV4   (RCC_PLLCFGR_PLLQ_2)
#define RCC_PLLCFGR_PLLP_DIV2   0

int main()
{
    /* Enable HSE */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    /* Setup and Enable PLL */
    uint32_t temp = RCC->PLLCFGR;
    temp &= ~(RCC_PLLCFGR_PLLSRC_Msk | RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk | RCC_PLLCFGR_PLLP_Msk | RCC_PLLCFGR_PLLQ_Msk);
    //       PLL Use HSE              PLLM == /25              PLLN == x192            PLLQ == /4              PLLP == /2
    temp |= (RCC_PLLCFGR_PLLSRC_HSE | RCC_PLLCFGR_PLLM_DIV25 | RCC_PLLCFGR_PLLN_x192 | RCC_PLLCFGR_PLLQ_DIV4 | RCC_PLLCFGR_PLLP_DIV2);
    RCC->PLLCFGR = temp;

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* Setup Clocks */
    // Clocks:   APB2 == /1             APB1 == /2            AHB == /1
    RCC->CFGR |= (RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_HPRE_DIV1);
    FLASH->ACR |= (FLASH_ACR_LATENCY_3WS | FLASH_ACR_PRFTEN);
    // Use PLL for System Clock
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (!(RCC->CFGR & RCC_CFGR_SWS));

    // System Core Clock should be 96 MHz (This makes APB1 48 MHz)
    SystemCoreClockUpdate();

    SysTick_Start();
    uart_init(115200);

    while (1) {
        // printk("Test\n");
    }

    return 0;
}
