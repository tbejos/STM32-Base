#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f4xx.h"

typedef struct {
    uint32_t pin;        // Pin Number
    uint32_t mode;       // Pin Mode
    uint32_t pupd;       // Internal Pull-Up Pull-Down
    uint32_t speed;      // GPIO Speed
    uint32_t alt;        // Alternate Function
} GPIO_InitStruct;

/* GPIO Value */
#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

/* GPIO Port Mode GPIOx_MODER */
#define MODE_INPUT              0x00
#define MODE_OUTPUT_PP          0x01
#define MODE_OUTPUT_OD          0x11    // Use upper nibble to hold OD
#define MODE_ALT_PP             0x02
#define MODE_ALT_OD             0x12    // Use upper nibble to hold OD
#define MODE_ANALOG             0x03

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
void gpio_init(GPIO_TypeDef *port, GPIO_InitStruct *init);
void gpio_toggle(GPIO_TypeDef *port, uint8_t pin);
void gpio_write_pin(GPIO_TypeDef *port, uint32_t pin, uint8_t value);
uint8_t gpio_read_pin(GPIO_TypeDef *port, uint8_t pin);

#endif /* _GPIO_H_ */
