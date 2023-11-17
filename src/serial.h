#ifndef SERIAL_H
#define SERIAL_H

/**
 * @brief Using the UART0 port, initialize the UART to communicate with the Serial Monitor.
 */
void serial_init();

/**
 * @brief Prints a formatted string to the Serial Monitor.
 */
int serial_printf(const char *fmt, ...);

/**
 * @brief Writes a buffer to the Serial Monitor.
 */
int serial_write(const char *buf, int len);

#endif // SERIAL_H
