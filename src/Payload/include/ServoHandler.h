
#ifndef SERVO_HANDLER_H
#define SERVO_HANDLER_H

#include <ESP32Servo.h>

void initServos();
void moveDisk1(char filter);
void moveDisk2(char filter);
void resetDisks();

#endif
