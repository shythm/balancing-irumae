#define F_CPU    16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>

#include "serial.h"
#include "mpu6050.h"
 
int main(void)
{
    serial_init();
    mpu6050_init();
    
    float accel_x, accel_y, accel_z;

    for (;;) 
    {
        accel_x = mpu6050_get_accel_x();
        accel_y = mpu6050_get_accel_y();
        accel_z = mpu6050_get_accel_z();

        serial_printf("accel_x: %+2.4f, accel_y: %+2.4f, accel_z: %+2.4f\r\n", accel_x, accel_y, accel_z);

        _delay_ms(10);
    }
}
