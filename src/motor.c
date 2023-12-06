#include "motor.h"

#include "avr/io.h"
#include "avr/iom128.h"

/** 
 * L298N Motor Driver Pinout
 * PA0: IN1
 * PA1: IN2
 * PB5: ENA(PWM, OC1A)
 * PA2: IN3
 * PA3: IN4
 * PB6: ENB(PWM, OC1B)
 */

inline void motor_set_direction(int motor, int forward) {
    if (motor == MOTOR_LEFT) {
        if (forward) {
            PORTA |= (1 << PA0);
            PORTA &= ~(1 << PA1);
        } else {
            PORTA &= ~(1 << PA0);
            PORTA |= (1 << PA1);
        }
    } else if (motor == MOTOR_RIGHT) {
        if (forward) {
            PORTA |= (1 << PA3);
            PORTA &= ~(1 << PA2);
        } else {
            PORTA &= ~(1 << PA3);
            PORTA |= (1 << PA2);
        }
    }
}

inline void motor_set_speed(int motor, int speed) {
    if (motor == MOTOR_LEFT) {
        OCR1A = speed;
    } else if (motor == MOTOR_RIGHT) {
        OCR1B = speed;
    }
}

void motor_init() {
    // set PA0, PA1, PA2, PA3 as output
    DDRA |= (1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3);
    // set PB5, PB6 as output
    DDRB |= (1 << PB5) | (1 << PB6);

    // set timer1 as fast PWM mode, 8-bit (WGM1 = 0101)
    // clear OC1A, OC1B on compare match (COM1A = 10, COM1B = 10, non-inverting mode)
    // set prescaler to 1 (CS1 = 001)
    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << WGM12) | (1 << CS10);

    // set initial duty cycle to 0
    OCR1A = 0x00;
    OCR1B = 0x00;

    // set initial direction to none
    PORTA &= ~((1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3));
}