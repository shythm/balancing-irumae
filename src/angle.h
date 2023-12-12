/**
 * @file angle.h
 * @author Seongho Lee (shythm@outlook.com)
 * @brief Angle measurement using MPU6050 with Timer2
 * @version 0.1
 * @date 2023-12-12
 */

#ifndef __ANGLE_H__
#define __ANGLE_H__

#include "mpu6050.h"

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

// callback function to get accel, gyro data
typedef void (*angle_get_data_t)(angle_accel_t *accel, angle_gyro_t *gyro, int urgent);

extern volatile float _angle;
static inline float angle_retrieve() {
    return _angle;
}

void angle_init(angle_get_data_t callback);
void angle_enabled(int enabled);

#endif
