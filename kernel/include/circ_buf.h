#ifndef _CIRC_BUF_H_
#define _CIRC_BUF_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *buf;   /**< Data for the buffer */
    uint8_t head;   /**< Where to Read Next */
    uint8_t tail;   /**< Where to Write Next */
    size_t size;    /**< Size of the buffer */
} circbuf_t;

uint8_t buffer_init(circbuf_t *buf, void *data, size_t len);
uint8_t valid_buf(circbuf_t *buf);
size_t buf_get_free(circbuf_t *buf);
size_t buffer_write(circbuf_t *buf, const void *data, size_t len);
size_t buf_get_full(circbuf_t *buf);
size_t buffer_read(circbuf_t *buf, uint8_t *data, size_t len_rx);
size_t buffer_skip(circbuf_t *buf, size_t len);
void *buffer_next_read_addr(circbuf_t *buf);
size_t buffer_next_read_len(circbuf_t *buf);

#endif /* _CIRC_BUF_H_ */
