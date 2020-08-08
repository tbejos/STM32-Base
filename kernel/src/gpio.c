#include "gpio.h"

#define BITS_PER_ALT 4
#define BITS_PER_MODE 2
#define BITS_PER_SPEED 2
#define BITS_PER_PUPD 2
#define BITS_PER_TYPE 1
#define GPIOS_PER_ALT_REG 8

GPIO_TypeDef *gpio_regs[] = {GPIOA, GPIOB, GPIOC};
const int gpio_en[] = {0x01, 0x02, 0x04, 0x08, 0x10};

void gpio_init(gpio_port port, uint8_t pin, uint8_t mode, uint8_t otype, uint8_t speed, uint8_t pupd, uint8_t alt)
{
    RCC->AHB1ENR |= gpio_en[port];

    gpio_regs[port]->MODER |= (mode << (pin * BITS_PER_MODE));
    gpio_regs[port]->OTYPER |= (otype << (pin * BITS_PER_TYPE));
    gpio_regs[port]->OSPEEDR |= (speed << (pin * BITS_PER_SPEED));
    gpio_regs[port]->PUPDR |= (pupd << (pin * BITS_PER_PUPD));

    uint8_t shift_val = pin % 8;
    uint8_t high = pin >= 8;

    gpio_regs[port]->AFR[high] |= (alt << (shift_val * BITS_PER_ALT));
}

void gpio_toggle(gpio_port port, uint8_t pin)
{
    gpio_regs[port]->ODR ^= (1 << pin);
}

void gpio_write(gpio_port port, uint8_t pin, uint8_t value)
{
    gpio_regs[port]->BSRR |= (value) ? (1 << pin) : (1 << (pin + 0x10));
}

// TODO: Read output gpio pin as well?
uint8_t gpio_read(gpio_port port, uint8_t pin)
{
    uint8_t value = gpio_regs[port]->IDR & (1 << pin);

    // Turn it into 0 or 1 non shifted
    return value >> pin;
}
