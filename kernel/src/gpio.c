#include "gpio.h"

#define BITS_PER_ALT 4
#define BITS_PER_MODE 2
#define BITS_PER_SPEED 2
#define BITS_PER_PUPD 2
#define BITS_PER_TYPE 1
#define GPIOS_PER_ALT_REG 8

void gpio_init(GPIO_TypeDef *port, uint8_t pin, uint8_t mode, uint8_t otype, uint8_t speed, uint8_t pupd, uint8_t alt)
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

    port->MODER |= (mode << (pin * BITS_PER_MODE));
    port->OTYPER |= (otype << (pin * BITS_PER_TYPE));
    port->OSPEEDR |= (speed << (pin * BITS_PER_SPEED));
    port->PUPDR |= (pupd << (pin * BITS_PER_PUPD));

    uint8_t shift_val = pin % 8;
    uint8_t high = pin >= 8;

    port->AFR[high] |= (alt << (shift_val * BITS_PER_ALT));
}

void gpio_toggle(GPIO_TypeDef *port, uint8_t pin)
{
    port->ODR ^= (1 << pin);
}

void gpio_set_high(GPIO_TypeDef *port, uint8_t pin)
{
    port->BSRR |= (1 << (pin + 0x10));
}
void gpio_set_low(GPIO_TypeDef *port, uint8_t pin)
{
    port->BSRR |= (1 << pin);
}

void gpio_set_value(GPIO_TypeDef *port, uint8_t pin, uint8_t value)
{
    value ? gpio_set_high(port, pin) : gpio_set_low(port, pin);
}

uint8_t gpio_read_input(GPIO_TypeDef *port, uint8_t pin)
{
    uint8_t value = port->IDR & (1 << pin);

    // Turn it into 0 or 1 non shifted
    return value >> pin;
}

uint8_t gpio_read_output(GPIO_TypeDef *port, uint8_t pin)
{
    uint8_t value = port->ODR & (1 << pin);

    // Turn it into 0 or 1 non shifted
    return value >> pin;
}
