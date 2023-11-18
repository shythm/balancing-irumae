#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

// MPU6050 register addresses
#define MPU6050_REG_ACCEL_XOUT_H    0x3B
#define MPU6050_REG_ACCEL_XOUT_L    0x3C
#define MPU6050_REG_ACCEL_YOUT_H    0x3D
#define MPU6050_REG_ACCEL_YOUT_L    0x3E
#define MPU6050_REG_ACCEL_ZOUT_H    0x3F
#define MPU6050_REG_ACCEL_ZOUT_L    0x40
#define MPU6050_REG_GYRO_XOUT_H     0x43
#define MPU6050_REG_GYRO_XOUT_L     0x44
#define MPU6050_REG_GYRO_YOUT_H     0x45
#define MPU6050_REG_GYRO_YOUT_L     0x46
#define MPU6050_REG_GYRO_ZOUT_H     0x47
#define MPU6050_REG_GYRO_ZOUT_L     0x48
#define MPU6050_REG_PWR_MGMT_1      0x6B

/**
 * @brief Initialize the MPU6050 sensor.
 */
void mpu6050_init();

/**
 * @brief Read a 16-bit data from the MPU6050 sensor.
 * 
 * @param reg register address to read
 * @return int16_t data stored in the register
 */
int16_t mpu6050_burst_read_sequence(uint8_t reg);

static inline uint16_t mpu6050_read_accel_x() {
    return mpu6050_burst_read_sequence(MPU6050_REG_ACCEL_XOUT_H);
}

static inline uint16_t mpu6050_read_accel_y() {
    return mpu6050_burst_read_sequence(MPU6050_REG_ACCEL_YOUT_H);
}

static inline uint16_t mpu6050_read_accel_z() {
    return mpu6050_burst_read_sequence(MPU6050_REG_ACCEL_ZOUT_H);
}

static inline uint16_t mpu6050_read_gyro_x() {
    return mpu6050_burst_read_sequence(MPU6050_REG_GYRO_XOUT_H);
}

static inline uint16_t mpu6050_read_gyro_y() {
    return mpu6050_burst_read_sequence(MPU6050_REG_GYRO_YOUT_H);
}

static inline uint16_t mpu6050_read_gyro_z() {
    return mpu6050_burst_read_sequence(MPU6050_REG_GYRO_ZOUT_H);
}

#endif // MPU6050_H
