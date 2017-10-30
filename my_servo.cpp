#include<Servo.h>
#include "my_servo.h"

Servo myservo;

void servo_init()
{
  myservo.attach(9); /*(pin番号,最小パルス幅,最大パルス幅*/
    myservo.writeMicroseconds(PULSE_MIN);
}

void turn_servo(double deg) {
  double pulse_deg,pulse;
  pulse_deg = (PULSE_MAX-PULSE_MIN)/360.0;  /*1度あたり何パルス幅増えるか*/
  pulse = pulse_deg*deg + PULSE_MIN;  /*degは何パルス幅か*/
  myservo.writeMicroseconds((int)pulse);
}
