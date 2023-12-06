#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1

void motor_init();
void motor_set_direction(int motor, int forward);
void motor_set_speed(int motor, int speed);

#endif