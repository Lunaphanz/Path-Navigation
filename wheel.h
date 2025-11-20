/*
 * wheel.h
 *
 *  Created on: Oct 30, 2025
 *      Author: Lou
 */

#ifndef WHEEL_H_
#define WHEEL_H_
//wheel motor
#define timeTurnLeft 410
#define timeTurnRight 390
#define leftSpeed TPM2_MOD
#define rightSpeed leftSpeed*0.88
#define TPM2_MOD 39999
//TODO: configuration
void setup_wheel();
void setup_TPM2();
void setup_PortB(); //PTB0,1,2,3 - right motor, PWM
void setup_PortC(); //PTC1,2 - left motor

//wheel motor logic
void go_straight();
void turn_left();
void turn_right();
void turn_around();
void stop();
void setMotorSpeed(float left, float right);

#endif /* WHEEL_H_ */
