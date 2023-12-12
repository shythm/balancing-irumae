#include "motor.h"
#include <stdlib.h>
#include <math.h>

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

void motor_set_direction(int motor, int forward) {
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

void motor_set_duty_ratio(int motor, float duty_ratio) {
    duty_ratio = fabsf(duty_ratio);
    // limit duty_ratio to [0, 1]
    if (duty_ratio > 1.0f) {
        duty_ratio = 1.0f;
    }

    // map duty_ratio[0, 1] to [MOTOR_MIN_POWER, 255]
    uint16_t power = (255 - MOTOR_MIN_POWER) * duty_ratio + MOTOR_MIN_POWER;

    if (power < 0) {
        power = 0;
    } else if (power > 255) {
        power = 255;
    }

    if (motor == MOTOR_LEFT) {
        OCR1A = (uint8_t)power;
    } else if (motor == MOTOR_RIGHT) {
        OCR1B = (uint8_t)power;
    }
}

void motor_control_enabled(int enabled) {
    if (enabled) {
        TIMSK |= (1 << OCIE0); // enable timer0 compare match interrupt
    } else {
        TIMSK &= ~(1 << OCIE0); // disable timer0 compare match interrupt
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

    // timer0 for motor control
    TCCR0 |= (1 << WGM01); // CTC(Clear Timer on Compare match) mode
    TCCR0 |= (1 << CS02); // prescaler 64
    // calculate OCR0 for 500us
    // 500us = OCR0 * 1 / (clk / prescaler)
    // OCR0 = 500us * (clk / prescaler) = 500us * (16MHz / 64) = 125
    OCR0 = 125;
}