#include <avr/io.h>
#include <avr/iom128.h>

#include "serial.h"

int main(void) {
    serial_init();

    DDRA = 0xFF;
    PORTA = 0xFF;
    
    int value = 0;
    for (;;) {
        serial_printf("Hello, world! %d\r\n", value);
        value++;
    }

    return 0;
}