#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f4xx.h"

/* GPIO Port */
typedef enum {GPIO_A = 0, GPIO_B = 1, GPIO_C = 2} gpio_port;

/* GPIO Value */
#define GPIO_PIN_RESET          0x00
#define GPIO_PIN_SET            0x01

/* GPIO Port Mode GPIOx_MODER */
#define MODE_INPUT              0x00
#define MODE_GP_OUTPUT          0x01
#define MODE_ALT_FN             0x02
#define MODE_ANALOG_INPUT       0x03

/* GPIO Output Type GPIOx_OTYPER */
#define OUTPUT_PUSH_PULL        0x00
#define OUTPUT_OPEN_DRAIN       0x01

/* GPIO Ouput Speed GPIOx_OSPEEDR */
#define OUTPUT_SPEED_LOW        0x00
#define OUTPUT_SPEED_MEDIUM     0x01
#define OUTPUT_SPEED_HIGH       0x02
#define OUTPUT_SPEED_VERY_HIGH  0x03

/* GPIO Pull-Up Pull-Down GPIOx_PUPDR */
#define PUPD_NONE               0x00
#define PUPD_PULL_UP            0x01
#define PUPD_PULL_DOWN          0x02

/* GPIO Alternate Function GPIOx_AFR(L/H) */
#define ALT0                    0x00
#define ALT1                    0x01
#define ALT2                    0x02
#define ALT3                    0x03
#define ALT4                    0x04
#define ALT5                    0x05
#define ALT6                    0x06
#define ALT7                    0x07
#define ALT8                    0x08
#define ALT9                    0x09
#define ALT10                   0x0A
#define ALT11                   0x0B
#define ALT12                   0x0C
#define ALT13                   0x0D
#define ALT14                   0x0E
#define ALT15                   0x0F

/* Function Prototypes */
void gpio_init(gpio_port port, uint8_t pin, uint8_t mode, uint8_t otype, uint8_t speed, uint8_t pupd, uint8_t alt);
void gpio_toggle(gpio_port port, uint8_t pin);
void gpio_write(gpio_port port, uint8_t pin, uint8_t value);
uint8_t gpio_read(gpio_port port, uint8_t pin);

#endif
