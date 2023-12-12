#define F_CPU    16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "serial.h"
#include "mpu6050.h"
#include "motor.h"
#include "angle.h"

volatile float error;
volatile float prev_error;

SIGNAL(MOTOR_CONTROL_IRQ) {
    static const float dt = 0.0005f; // interrupt for every 500us
    static const float kp = 0.003f; // proportional gain
    static const float kd = 0.0f; // derivative gain

    float error_diff = (error - prev_error) * dt;

    float duty_ratio = kp * error + kd * error_diff;

    motor_set_direction(MOTOR_LEFT, duty_ratio < 0);
    motor_set_direction(MOTOR_RIGHT, duty_ratio < 0);
    motor_set_duty_ratio(MOTOR_LEFT, duty_ratio);
    motor_set_duty_ratio(MOTOR_RIGHT, duty_ratio);

    prev_error = error;
}

int main(void)
{
    sei();

    serial_init();
    mpu6050_init();
    motor_init();
    angle_init();

    angle_enabled(1);
    motor_control_enabled(1);

    for (;;) {
        error = angle_retrieve();
        serial_printf("pitch: %+2.4f, OCR1A: %3d \r\n", angle_retrieve(), OCR1A);
    }
}
