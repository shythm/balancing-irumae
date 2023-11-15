#include <avr/io.h>
#include <avr/iom128.h>

int main(void) {
    DDRA = 0xFF;
    PORTA = 0xFF;
    
    for (;;) { }

    return 0;
}