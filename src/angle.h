/**
 * @file angle.h
 * @author Seongho Lee
 * @brief Angle measurement using gyro/accelerometer with Kalman filter
 * 
 * @details
 * [ Operation Flow ]
 * 1. first call angle_init() with callback function to retrieve data from MPU6050
 * 2. call angle_enabled(TRUE) to enable angle measurement
 *    -> then, timer2 interrupt will calculate angle every cycle
 * 3. call angle_retrieve() to get angle in degree
 */

#ifndef __ANGLE_H__
#define __ANGLE_H__

typedef struct {
    float x;
    float y;
    float z;
} angle_accel_t;

typedef struct {
    float x;
    float y;
    float z;
} angle_gyro_t;

/**
 * @brief callback function to get accel, gyro data.
 * (1) with urgent flag 1, the data will be retrieved immediately(not waiting for new data from MPU6050).
 * (2) with urgent flag 0, this callback function should retreive data(angle and gyro) from MPU6050.
 */
typedef void (*angle_get_data_t)(angle_accel_t *accel, angle_gyro_t *gyro, int urgent);

extern float _angle;

/**
 * @brief get angle in degree applied Kalman filter
 * 
 * @return float angle in degree
 */
static inline float angle_retrieve() {
    return _angle;
}

/**
 * @brief initialize angle measurement. this function prepares timer2 interrupt to apply Kalman filter.
 * 
 * @param callback callback function to get accel, gyro data.
 */
void angle_init(angle_get_data_t callback);

/**
 * @brief enable/disable angle measurement.
 * 
 * @param enabled 
 */
void angle_enabled(int enabled);

#endif
