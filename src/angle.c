#include "angle.h"

#include <stdlib.h>
#include <math.h>

#include "avr/io.h"
#include "avr/iom128.h"
#include "avr/interrupt.h"

volatile angle_get_data_t _angle_get_data;
volatile float _angle;

#define ANGLE_IRQ_PERIOD        0.004f
#define ANGLE_STD_DEV_OF_GYRO   6
#define ANGLE_STD_DEV_OF_ACCEL  8

SIGNAL(TIMER2_COMP_vect) {
    static float theta_gyro;
    static float k_predict, k_uncertainty;

    angle_accel_t accel;
    angle_gyro_t gyro;
    _angle_get_data(&accel, &gyro, 1);

    float accel_x = accel.x - 0.05f;
    float accel_y = accel.y + 0.01f;
    float accel_z = accel.z + 0.01f;

    // calculate pitch by gyroscope
    theta_gyro = theta_gyro + gyro.x * ANGLE_IRQ_PERIOD;

    // calculate pitch by accelerometer
    float theta_accel = atanf(accel_y / sqrtf(accel_x * accel_x + accel_z * accel_z)) * 180.0f / M_PI;

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

void angle_init(angle_get_data_t callback) {
    // timer2 for measurement angle
    TCCR2 |= (1 << WGM21); // CTC mode
    TCCR2 |= (1 << CS22);  // prescaler 256
    // calculate OCR2 for 5ms
    // 4ms = OCR2 * 1 / (clk / prescaler)
    // OCR2 = 4ms * (clk / prescaler) = 4ms * (16MHz / 256) = 250
    OCR2 = 250;

    _angle_get_data = callback;
}

void angle_enabled(int enabled) {
    if (enabled) {
        TIMSK |= (1 << OCIE2); // enable timer2 compare match interrupt
    } else {
        TIMSK &= ~(1 << OCIE2); // disable timer2 compare match interrupt
    }
}
