#include "gpio.h"

#define BITS_PER_ALT 4
#define BITS_PER_MODE 2
#define BITS_PER_SPEED 2
#define BITS_PER_PUPD 2
#define GPIOS_PER_ALT_REG 8

void gpio_init(GPIO_TypeDef *port, GPIO_InitStruct *init)
{
    // TODO: Kind of gross
    switch ((uint32_t)port) {
    case (uint32_t)GPIOA:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        break;
    case (uint32_t)GPIOB:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        break;
    case (uint32_t)GPIOC:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        break;
    case (uint32_t)GPIOD:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        break;
    case (uint32_t)GPIOE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
        break;
    }

    uint32_t temp = 0;

    /* Clear existing value then set new one */
    temp = port->MODER;
    temp &= ~(0x3 << (init->pin * BITS_PER_MODE));
    temp |= init->mode << (init->pin * BITS_PER_MODE);
    port->MODER = temp;

    temp = port->PUPDR;
    temp &= ~(0x3 << (init->pin * BITS_PER_PUPD));
    temp |= init->mode << (init->pin * BITS_PER_PUPD);
    port->PUPDR = temp;

    if (init->mode == MODE_OUTPUT_PP || init->mode == MODE_OUTPUT_OD
        || init->mode == MODE_ALT_PP || init->mode == MODE_ALT_OD) {

        temp = port->OTYPER;
        temp &= ~(0x1 << init->pin);
        temp |= (init->mode >> 4) << init->pin;
        port->OTYPER = temp;

        temp = port->OSPEEDR;
        temp &= ~(0x3 << (init->pin * BITS_PER_SPEED));
        temp |= (init->speed << (init->pin * BITS_PER_SPEED));
        port->OSPEEDR = temp;
    }

    if (init->mode == MODE_ALT_PP || init->mode == MODE_ALT_OD) {
        temp = port->AFR[init->pin >> 3];
        temp &= ~(0xF << ((init->pin & 0x7) * BITS_PER_ALT));
        temp |= (init->alt << ((init->pin & 0x7) * BITS_PER_ALT));
        port->AFR[init->pin >> 3] = temp;
    }
}

void gpio_toggle(GPIO_TypeDef *port, uint8_t pin)
{
    port->ODR ^= (1 << pin);
}

void gpio_write_pin(GPIO_TypeDef *port, uint32_t pin, uint8_t value)
{
    port->BSRR = (value) ? pin : pin << 0x10;
    return;
}

uint8_t gpio_read_pin(GPIO_TypeDef *port, uint8_t pin)
{
    return (port->IDR & (1 << pin)) ? 1 : 0;
}
