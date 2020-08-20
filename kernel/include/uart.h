#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

/** @brief Maximum number of elements in buffer */
#define MAX_BUF 256

struct buf {
    uint8_t head;
    uint8_t tail;
    uint8_t buf[MAX_BUF];
};

void uart_init(int baud);
int uart_put_byte(char c);
int uart_get_byte(char *c);

// void uart_flush();

#endif /* _UART_H_ */
