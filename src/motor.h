#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1

void motor_init();
void motor_set_direction(int motor, int forward);
void motor_set_speed(int motor, int speed);
void motor_control_enabled(int enabled);

/**
 * For motor control, we use timer0 compare match interrupt.
 */
#define MOTOR_CONTROL_IRQ   TIMER0_COMP_vect

#endif