#define F_CPU    16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <stdlib.h>

#include "serial.h"
#include "mpu6050.h"
#include "motor.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

int main(void)
{
    serial_init();
    mpu6050_init();
    motor_init();
    
    float accel_x, accel_y, accel_z;
    const float kp = 75.f;

    for (;;) 
    {
        accel_x = mpu6050_get_accel_x();
        accel_y = mpu6050_get_accel_y();
        accel_z = mpu6050_get_accel_z();

        motor_set_direction(MOTOR_LEFT, accel_y < 0);
        motor_set_direction(MOTOR_RIGHT, accel_y < 0);
        motor_set_speed(MOTOR_LEFT, ABS(accel_y) * kp);
        motor_set_speed(MOTOR_RIGHT, ABS(accel_y) * kp);

        serial_printf("accel_x: %+2.4f, accel_y: %+2.4f, accel_z: %+2.4f\r\n", accel_x, accel_y, accel_z);

        _delay_ms(10);
    }
}
