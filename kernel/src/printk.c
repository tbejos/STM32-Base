#include "printk.h"
#include "uart.h"
#include <stdarg.h>

// Allows for numbers with 64 digits/letters
#define MAXBUF (sizeof(uint32_t))

static char digits[] = "0123456789abcdef";

void uart_wrapper(char c)
{
    uart_put_byte(c);
}

/**
 * @brief Prints a number
 *
 * @param base  8, 10, 16
 * @param num   Number to print
 * */
static void printnumk(uint8_t base, uint32_t num) {
    int8_t *prefix = 0;
    int8_t buf[MAXBUF];
    int8_t *ptr = &buf[MAXBUF - 1];

    // standard radius prefixes
    if (base == 8) {
        prefix = (int8_t *)"0";
    } else if (base == 16) {
        prefix = (int8_t *)"0x";
    }

    do {
        *ptr-- = digits[num % base];
        num /= base;
    } while (num != 0);

    if (prefix) {
        while (*prefix)
            uart_wrapper(*prefix++);
    }

    while (++ptr != &buf[MAXBUF])
        uart_wrapper(*ptr);
}

/**
 * @brief A kernel printf() function for logging
 *
 * @param   fmt The format string
 * @param[in]   <unamed>    variable input
 *
 * @return 0 or success or -1 on failure;
 */
int printk(const char *fmt, ...) {
    va_list args;
    // set up va_list and print it
    va_start(args, fmt);

    uint32_t num;
    int8_t byte;
    int8_t *byte_ptr;

    // loop through format string looking for formatting
    while (*fmt) {
        if (*fmt != '%') {
            uart_wrapper(*fmt++);
            continue;
        }

        fmt++;

        // handle formatting
        switch(*fmt) {
        case 'd':   // Signed Decimal
            num = va_arg(args, int32_t);

            if ((int32_t)num < 0) {
                uart_wrapper('-');
                printnumk(10, -(int32_t)num);
            } else {
                printnumk(10, num);
            }
            break;

        case 'u':   // Unisgned Decimal
            num = va_arg(args, uint32_t);
            printnumk(10, num);
            break;

        case 'o':   // Octal
            num = va_arg(args, uint32_t);
            printnumk(8, num);
            break;

        case 'x':   // Hexadecimal
        case 'p':   // Pointer
            num = va_arg(args, uint32_t);
            printnumk(16, num);
            break;

        case 's':   // String
            byte_ptr = (int8_t*)va_arg(args, int32_t);
            while (*byte_ptr) {
                uart_wrapper(*byte_ptr);
                byte_ptr++;
            }
            break;

        case 'c':   // Character
            byte = va_arg(args, int32_t);
            uart_wrapper(byte);
            break;

        case '%':
            uart_wrapper('%');
            break;

        default:
            va_end(args);
            return -1;

        }

        fmt++;
    }

    va_end(args);
    return 0;
}
