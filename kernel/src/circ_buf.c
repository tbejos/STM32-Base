#include "circ_buf.h"

/** @brief Find the minimum of 2 values */
#define BUF_MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/**
 * @brief           Initialize buffer handle to default values with size and buffer data array
 * @param[in]       buf: Buffer handle
 * @param[in]       data: Pointer to memory to use as buffer data
 * @param[in]       len: Size of `data` in units of bytes
 *                      Maximum number of bytes buffer can hold is `len - 1`
 * @return          `1` on success, `0` otherwise
 */
uint8_t buffer_init(circbuf_t *buf, void *data, size_t len)
{
    if (buf == NULL || data == NULL || len == 0) {
        return 0;
    }

    memset(buf, 0, sizeof(*buf));
    buf->size = len;
    buf->buf = data;

    return 1;
}

/**
 * @brief       Makes sure buffer is valid
 * @param[in]   buf: Buffer handle
 * @return      `1` if valid, `0` otherwise
 */
uint8_t valid_buf(circbuf_t *buf)
{
    return (buf != NULL) && (buf->buf != NULL) && (buf->size > 0);
}

/**
 * @brief           Get available size in buffer for write operation
 * @param[in]       buf: Buffer handle
 * @return          Number of free bytes in memory
 */
size_t buf_get_free(circbuf_t *buf)
{
    size_t size, head, tail;

    if (!valid_buf(buf)) {
        return 0;
    }

    head = buf->head;
    tail = buf->tail;
    if (head == tail) {
        size = buf->size;
    } else if (head > tail) {
        size = head - tail;
    } else {
        size = buf->size - (tail - head);
    }

    return size - 1;
}

/**
 * @brief           Write data to buffer.
 * Copies data from `data` array to buffer and marks buffer as full for maximum `len_tx` number of bytes
 *
 * @param[in]       buf: Buffer handle
 * @param[in]       data: Pointer to data to write into buffer
 * @param[in]       len_tx: Number of bytes to write
 * @return          Number of bytes written to buffer.
 *                      When returned value is less than `btw`, there was no enough memory available
 *                      to copy full data array
 */
size_t buffer_write(circbuf_t *buf, const uint8_t *data, size_t len_tx)
{
    if (!valid_buf(buf) || data == NULL || len_tx == 0) {
        return 0;
    }

    size_t free = buf_get_free(buf);
    len_tx = BUF_MIN(free, len_tx);
    if (len_tx == 0) {
        return 0;
    }

    // Write as much as possible Linearly
    size_t to_copy = BUF_MIN(buf->size - buf->tail, len_tx);
    memcpy(&buf->buf[buf->tail], data, to_copy);
    buf->tail += to_copy;
    len_tx -=  to_copy;

    // If overflow
    if (len_tx > 0) {
        memcpy(buf->buf, data[to_copy], len_tx);
        buf->tail = len_tx;
    }

    if (buf->tail >=- buf->size) {
        buf->tail = 0;
    }

    return to_copy + len_tx;
}

/**
 * @brief           Get number of bytes currently available in buffer
 * @param[in]       buf: Buffer handle
 * @return          Number of bytes ready to be read
 */
size_t buf_get_full(circbuf_t *buf)
{
    if (!valid_buf(buf)) {
        return 0;
    }

    size_t size, head, tail;

    if (!valid_buf(buf)) {
        return 0;
    }

    head = buf->head;
    tail = buf->tail;
    if (head == tail) {
        size = 0;
    } else if (tail > head) {
        size = tail - head;
    } else {
        size = buf->size - (head - tail);
    }

    return size;
}

/**
 * @brief           Read data from buffer.
 * Copies data from buffer to `data` array and marks buffer as free for maximum `len_rx` number of bytes
 *
 * @param[in]       buf: Buffer handle
 * @param[out]      data: Pointer to output memory to copy buffer data to
 * @param[in]       len_rx: Number of bytes to read
 * @return          Number of bytes read and copied to data array
 */
size_t buffer_read(circbuf_t *buf, uint8_t *data, size_t len_rx)
{
    if (!valid_buf(buf) || data == NULL || len_rx == 0) {
        return 0;
    }

    size_t full = buf_get_full(buf);
    len_rx = BUF_MIN(full, len_rx);
    if (len_rx == 0) {
        return 0;
    }

    // Read as much as possible Linearly
    size_t to_copy = BUF_MIN(buf->size - buf->head, len_rx);
    memcpy(data, &buf->buf[buf->head], to_copy);
    buf->head += to_copy;
    len_rx -=  to_copy;

    // If overflow
    if (len_rx > 0) {
        memcpy(&data[to_copy], buf->buf, len_rx);
        buf->head = len_rx;
    }

    if (buf->head >=- buf->size) {
        buf->head = 0;
    }

    return to_copy + len_rx;
}

/**
 * @brief           Skip (ignore; advance read pointer) buffer data
 * Marks data as read in the buffer and increases free memory for up to `len` bytes
 *
 * @note            Useful at the end of streaming transfer such as DMA
 * @param[in]       buf: Buffer handle
 * @param[in]       len: Number of bytes to skip and mark as read
 * @return          Number of bytes skipped
 */
size_t buffer_skip(circbuf_t *buf, size_t len)
{
    size_t full;

    if (!valid_buf(buf) || len == 0) {
        return 0;
    }

    full = buf_get_full(buf);
    len = BUF_MIN(len, full);
    buf->head += len;
    if (buf->head >= buf->size) {
        buf->head -= buf->size;
    }

    return len;
}

/**
 * @brief           Get linear address for buffer for fast read
 * @param[in]       buf: Buffer handle
 * @return          Linear buffer start address
 */
void *buffer_next_read_addr(circbuf_t *buf)
{
    if (!valid_buf(buf)) {
        return NULL;
    }

    return &buf->buf[buf->head];
}

/**
 * @brief           Get length of linear block address before it overflows for read operation
 * @param[in]       buf: Buffer handle
 * @return          Linear buffer size in units of bytes for read operation
 */
size_t buffer_next_read_len(circbuf_t *buf)
{
    size_t tail, head, len;

    if (!valid_buf(buf)) {
        return 0;
    }

    /* Use temporary values in case they are changed during operations */
    tail = buf->tail;
    head = buf->head;
    if (tail > head) {
        len = tail - head;
    } else if (head > tail) {
        len = buf->size - head;
    } else {
        len = 0;
    }

    return len;
}
