#include "uart.h"
#include "gpio.h"

/** @breif Find the minimum of 2 values */
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define APB1_CLK            (SystemCoreClock >> 1)

static struct buf rx_buf = { 0, 0, {0}};    /**< Recieve Buffer */
static struct buf tx_buf = { 0, 0, {0}};    /**< Transmit Buffer */

void uart_init(int baud)
{
    GPIO_InitStruct usart2 = {0};
    usart2.pupd = PUPD_NONE;
    usart2.speed = OUTPUT_SPEED_LOW;
    usart2.alt = ALT7;

    // TX Initialize
    usart2.pin = 2;
    usart2.mode = MODE_ALT_PP;
    gpio_init(GPIOA, &usart2);

    // RX Initialize
    usart2.pin = 3;
    usart2.mode = MODE_ALT_OD;
    gpio_init(GPIOA, &usart2);

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    uint32_t temp = USART2->BRR;
    temp &= ~(USART_BRR_DIV_Mantissa_Msk | USART_BRR_DIV_Fraction_Msk);
    temp = (APB1_CLK / baud) + 1;
    USART2->BRR = temp;

    USART2->CR1 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE);

    NVIC_EnableIRQ(USART2_IRQn);
}

int uart_put_byte(char c)
{
    struct buf *p = &tx_buf;
    if (p->head - p->tail == MAX_BUF - 1)
        return -1;

    p->buf[p->head] = c;
    p->head++;

    if (!(USART2->CR1 & USART_CR1_TXEIE)) {
        USART2->CR1 |= USART_CR1_TXEIE;
    }
    return 0;
}

int uart_get_byte(char *c)
{
    struct buf *p = &rx_buf;
    if (p->head == p->tail)
        return -1;

    *c = p->buf[p->tail];
    p->tail++;
    return 0;
}

void USART2_IRQHandler()
{
    struct buf *p;
    NVIC_ClearPendingIRQ(USART2_IRQn);

    if (USART2->SR & USART_SR_RXNE) {
        USART2->SR &= ~USART_SR_RXNE;
        p = &rx_buf;
        if (p->head - p->tail <= MAX_BUF - 1) {
            p->buf[p->head] = USART2->DR;
            p->head++;
        }
    }

    if (USART2->SR & USART_SR_TXE) {
        USART2->SR &= ~USART_SR_TXE;
        p = &tx_buf;
        // Disable Interrupt if nothing to transmit
        if (p->head == p->tail) {
            USART2->CR1 &= ~USART_CR1_TXEIE;
        } else {
            uint8_t num_times = MIN(p->head - p->tail, 16);
            for (uint8_t i = 0; i < num_times; i++) {
                while (!(USART2->SR & USART_SR_TXE));
                USART2->DR = p->buf[p->tail];
                p->tail++;
            }
        }
    }
}

/*
void uart_flush()
{
    struct buf *p = &tx_buf;
    uint8_t num_times = p->head - p->tail;
    for (uint8_t i = 0; i < num_times; i++) {
        while(!)
    }
}
*/
