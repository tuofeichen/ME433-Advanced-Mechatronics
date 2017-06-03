#ifndef _MOTOR_H
#define _MOTOR_H
#include<xc.h>  
#include<sys/attribs.h>  // __ISR macro

#define PERIOD 1200

void motor_init();
void motor_set_speed(int motor, int pct, int dir);

#endif