/**
 * @file serial.h
 * @author Seongho Lee
 * @brief This module performs to print formatted serial output
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

/**
 * @brief Using the UART0 port, initialize the UART to communicate with the Serial Monitor.
 */
void serial_init();

/**
 * @brief Prints a formatted string to the Serial Monitor.
 */
int serial_printf(const char *fmt, ...);

#endif // SERIAL_H
