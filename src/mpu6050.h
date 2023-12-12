/**
 * @file mpu6050.h
 * @author Seongho Lee (shythm@outlook.com)
 * @brief MPU6050 sensor driver
 * @date 2023-11-18
 */

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
#define MPU6050_REG_GYRO_CONFIG     0x1B
#define MPU6050_REG_ACCEL_CONFIG    0x1C
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

#define MPU6050_ACC_FULL_SCALE_2G
#define MPU6050_GYRO_FULL_SCALE_250

// MPU6050 accelerometer full scale range
#if defined(MPU6050_ACC_FULL_SCALE_2G)
#define MPU6050_ACC_LSB_SENSITIVITY 16384.f
#define MPU6050_AFS_SEL 0x00
#elif defined(MPU6050_ACC_FULL_SCALE_4G)
#define MPU6050_ACC_LSB_SENSITIVITY 8192.f
#define MPU6050_AFS_SEL 0x08
#elif defined(MPU6050_ACC_FULL_SCALE_8G)
#define MPU6050_ACC_LSB_SENSITIVITY 4096.f
#define MPU6050_AFS_SEL 0x10
#elif defined(MPU6050_ACC_FULL_SCALE_16G)
#define MPU6050_ACC_LSB_SENSITIVITY 2048.f
#define MPU6050_AFS_SEL 0x18
#endif

// MPU6050 gyroscope full scale range
#if defined(MPU6050_GYRO_FULL_SCALE_250)
#define MPU6050_GYRO_LSB_SENSITIVITY 131.f
#define MPU6050_FS_SEL 0x00
#elif defined(MPU6050_GYRO_FULL_SCALE_500)
#define MPU6050_GYRO_LSB_SENSITIVITY 65.5f
#define MPU6050_FS_SEL 0x08
#elif defined(MPU6050_GYRO_FULL_SCALE_1000)
#define MPU6050_GYRO_LSB_SENSITIVITY 32.8f
#define MPU6050_FS_SEL 0x10
#elif defined(MPU6050_GYRO_FULL_SCALE_2000)
#define MPU6050_GYRO_LSB_SENSITIVITY 16.4f
#define MPU6050_FS_SEL 0x18
#endif

#define MPU6050_GET_ACCEL(reg) \
    (mpu6050_burst_read_sequence(reg) / MPU6050_ACC_LSB_SENSITIVITY)

/**
 * @brief Get the x-axis acceleration.
 * @return float m/s^2
 */
static inline float mpu6050_get_accel_x() {
    return MPU6050_GET_ACCEL(MPU6050_REG_ACCEL_XOUT_H);
}

/**
 * @brief Get the y-axis acceleration.
 * @return float m/s^2
 */
static inline float mpu6050_get_accel_y() {
    return MPU6050_GET_ACCEL(MPU6050_REG_ACCEL_YOUT_H);
}

/**
 * @brief Get the z-axis acceleration.
 * @return float m/s^2
 */
static inline float mpu6050_get_accel_z() {
    return MPU6050_GET_ACCEL(MPU6050_REG_ACCEL_ZOUT_H);
}

#define MPU6050_GET_GYRO(reg) \
    (mpu6050_burst_read_sequence(reg) / MPU6050_GYRO_LSB_SENSITIVITY)

/**
 * @brief Get the x-axis angular velocity.
 * @return float degree/s
 */
static inline float mpu6050_get_gyro_x() {
    return MPU6050_GET_GYRO(MPU6050_REG_GYRO_XOUT_H);
}

/**
 * @brief Get the y-axis angular velocity.
 * @return float degree/s
 */
static inline float mpu6050_get_gyro_y() {
    return MPU6050_GET_GYRO(MPU6050_REG_GYRO_YOUT_H);
}

/**
 * @brief Get the z-axis angular velocity.
 * @return float degree/s
 */
static inline float mpu6050_get_gyro_z() {
    return MPU6050_GET_GYRO(MPU6050_REG_GYRO_ZOUT_H);
}

#endif // MPU6050_H
