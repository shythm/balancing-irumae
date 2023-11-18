/**
 * @file mpu6050.c
 * @author Seongho Lee (shythm@outlook.com)
 * @brief MPU6050 sensor driver
 * @date 2023-11-18
 */

#include "mpu6050.h"
#include "i2c.h"

#define MPU6050_ADDRESS 0x68
#define MPU6050_SLA_W   (MPU6050_ADDRESS << 1)
#define MPU6050_SLA_R   ((MPU6050_ADDRESS << 1) | 0x01)

void mpu6050_init() {
    // init i2c
    i2c_init();

    // AFS_SEL Full Scale Range LSB Sensitivity
    i2c_start();
    i2c_write(MPU6050_SLA_W);
    i2c_write(MPU6050_REG_ACCEL_CONFIG);
    i2c_write(MPU6050_AFS_SEL);

    // FS_SEL Full Scale Range LSB Sensitivity
    i2c_start();
    i2c_write(MPU6050_SLA_W);
    i2c_write(MPU6050_REG_GYRO_CONFIG);
    i2c_write(MPU6050_FS_SEL);

    // Wake up the MPU6050 sensor
    i2c_start();
    i2c_write(MPU6050_SLA_W);
    i2c_write(MPU6050_REG_PWR_MGMT_1);
    i2c_write(0x00); // Set the power management register to 0 (wakes up the sensor)
    i2c_stop();
}

int16_t mpu6050_burst_read_sequence(uint8_t reg) {
    int16_t data = 0;

    i2c_start();
    i2c_write(MPU6050_SLA_W);
    i2c_write(reg);
    i2c_stop();

    i2c_start();
    i2c_write(MPU6050_SLA_R);
    data = (i2c_read(1) << 8);
    data |= i2c_read(0);
    i2c_stop();
    
    return data;
}
