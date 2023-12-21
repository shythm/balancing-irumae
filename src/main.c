#define F_CPU    16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdlib.h>
#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "serial.h"
#include "mpu6050.h"
#include "motor.h"
#include "angle.h"

#define TRUE    1
#define FALSE   0

float error;
float prev_error;

SIGNAL(MOTOR_CONTROL_IRQ) {
    PORTC |= (1 << PC1); // for debugging

    static const float dt = 0.008f; // 8ms
    static const float kp = 0.008f; // proportional gain
    static const float kd = 0.001f; // derivative gain
    static const float ki = 0.001f; // integral gain

    static float error_sum = 0.0f;

    float error = angle_retrieve();
    float error_diff = (error - prev_error) * dt;
    error_sum += error * dt;
    if (error_sum > 100.f) error_sum = 100.f;

    float duty_ratio = kp * error + kd * error_diff + ki * error_sum;

    motor_set_direction(MOTOR_LEFT, duty_ratio < 0);
    motor_set_direction(MOTOR_RIGHT, duty_ratio < 0);
    motor_set_duty_ratio(MOTOR_LEFT, duty_ratio);
    motor_set_duty_ratio(MOTOR_RIGHT, duty_ratio);

    prev_error = error;

    PORTC &= ~(1 << PC1); // for debugging
}

void angle_get_data(angle_accel_t *accel, angle_gyro_t *gyro, int urgent) {
    static angle_accel_t _accel;
    static angle_gyro_t _gyro;

    if (!urgent) {
        _accel.x = mpu6050_get_accel_x();
        _accel.y = mpu6050_get_accel_y();
        _accel.z = mpu6050_get_accel_z();
        _gyro.x = mpu6050_get_gyro_x();
    }

    if (accel) *accel = _accel;
    if (gyro) *gyro = _gyro;
}

int main(void) {
    sei();
    DDRC = 0xff; // for debugging

    serial_init();
    mpu6050_init();
    angle_init(angle_get_data);
    motor_init();

    angle_enabled(TRUE);
    motor_control_enabled(TRUE);

    for (;;) {
        angle_get_data(NULL, NULL, FALSE);
        serial_printf("bottom:-30,angle:%2.4f,top:30\r\n", angle_retrieve());
    }
}
