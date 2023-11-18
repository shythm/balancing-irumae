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
    
    uint16_t accel_x, accel_y, accel_z;

    for (;;) 
    {
        accel_x = mpu6050_read_accel_x();
        accel_y = mpu6050_read_accel_y();
        accel_z = mpu6050_read_accel_z();

        serial_printf("accel_x: %+6d, accel_y: %+6d, accel_z: %+6d\r", accel_x, accel_y, accel_z);

        _delay_ms(10);
    }
}