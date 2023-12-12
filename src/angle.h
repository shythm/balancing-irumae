/**
 * @file angle.h
 * @author Seongho Lee (shythm@outlook.com)
 * @brief Angle measurement using MPU6050 with Timer2
 * @version 0.1
 * @date 2023-12-12
 */

#ifndef __ANGLE_H__
#define __ANGLE_H__

#include "avr/io.h"
#include "avr/iom128.h"
#include "avr/interrupt.h"

#include <math.h>
#include "mpu6050.h"

static float _angle;

#define ANGLE_IRQ_PERIOD        0.004f
#define ANGLE_STD_DEV_OF_GYRO   4
#define ANGLE_STD_DEV_OF_ACCEL  3

SIGNAL(TIMER2_COMP_vect) {
    static float accel_x, accel_y, accel_z;
    static float theta_gyro, theta_accel;
    static float k_predict, k_uncertainty;

    accel_x = mpu6050_get_accel_x() - 0.05f;
    accel_y = mpu6050_get_accel_y() + 0.01f;
    accel_z = mpu6050_get_accel_z() + 0.01f;

    // calculate pitch by gyroscope
    theta_gyro = theta_gyro + mpu6050_get_gyro_x() * ANGLE_IRQ_PERIOD;

    // calculate pitch by accelerometer
    theta_accel = atanf(accel_y / sqrtf(accel_x * accel_x + accel_z * accel_z)) * 180.0f / M_PI;

    /**
     * calculate pitch by Kalman filter (complementary filter)
     * https://youtu.be/5HuN9iL-zxU?si=seBpv3OUKMvicVIw
     */
    // 1. predict the current of the system
    k_predict = k_predict + ANGLE_IRQ_PERIOD * theta_gyro;
    // 2. calculate the uncertainty of the system
    k_uncertainty = k_uncertainty + ANGLE_IRQ_PERIOD * ANGLE_STD_DEV_OF_GYRO * ANGLE_STD_DEV_OF_GYRO;
    // 3. calculate the Kalman gain from the uncertainties on the predictions and measurements
    float k_gain = k_uncertainty / (k_uncertainty + ANGLE_STD_DEV_OF_ACCEL * ANGLE_STD_DEV_OF_ACCEL);
    // 4. update the predicted state of the system with the measurement of the state through the Kalman gain
    k_predict = k_predict + k_gain * (theta_accel - k_predict);
    // 5. update the uncertainty of the predicted state
    k_uncertainty = (1 - k_gain) * k_uncertainty;

    _angle = k_predict;
}

static void angle_init()
{
    // timer2 for measurement angle
    TCCR2 |= (1 << WGM21); // CTC mode
    TCCR2 |= (1 << CS22);  // prescaler 256
    // calculate OCR2 for 5ms
    // 4ms = OCR2 * 1 / (clk / prescaler)
    // OCR2 = 4ms * (clk / prescaler) = 4ms * (16MHz / 256) = 250
    OCR2 = 250;
}

static void angle_enabled(int enabled) {
    if (enabled) {
        TIMSK |= (1 << OCIE2); // enable timer2 compare match interrupt
    } else {
        TIMSK &= ~(1 << OCIE2); // disable timer2 compare match interrupt
    }
}

static inline float angle_retrieve() {
    return _angle;
}

#endif
