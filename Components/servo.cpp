// File will do Servo Turns and Init the servo
#include "defines.h"

#define MIDDLE 90
#define LEFT 0
#define RIGHT 180

void init_servo(Servo myservo){}

void servo_left(Servo myservo)
{
    myservo.write(LEFT);
}

void servo_right(Servo myservo)
{
    myservo.write(RIGHT);
}
