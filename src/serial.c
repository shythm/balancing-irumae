/**
 * @file serial.c
 * @author Seongho Lee
 * @brief This module performs to print formatted serial output
 * 
 * @details
 * 1. using the UART0, initialize the UART to communicate with the serial monitor
 *    (such as minicom, arduino serial monitor, ...).
 * 2. prints a formatted string using vsprintf in stdio.h
 */

#include <stdio.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/iom128.h>

#define UART0_BUFSIZE 128

void serial_init() {
    // using UART0
    // set baud rate to 9600
    UBRR0H = 0;
    UBRR0L = 103;
    // enable TX
    UCSR0B = (1 << TXEN0); 
    // 8-bit data
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static inline void serial_putchar(unsigned char c) {
    // UDREn: USART Data Register Empty
    while (!(UCSR0A & (1 << UDRE0))); // wait until the buffer is empty
    UDR0 = c;
}

int serial_printf(const char *fmt, ...) {
    static char buf[UART0_BUFSIZE];

    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i = 0; i < ret; i++) {
        serial_putchar(buf[i]);
    }

    return ret;
}
