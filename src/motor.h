#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1

void motor_init();
void motor_set_direction(int motor, int forward);
void motor_control_enabled(int enabled);
void motor_set_duty_ratio(int motor, float duty_ratio);

/**
 * For motor control, we use timer0 compare match interrupt.
 */
#define MOTOR_CONTROL_IRQ   TIMER0_COMP_vect

/**
 * Minimum power for motor to move.
 */
#define MOTOR_MIN_POWER 110

#endif