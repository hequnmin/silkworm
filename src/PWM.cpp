// Pulse Width Modulation
#include "PWM.h"

PWM::PWM() {

}

PWM::~PWM() {
  
}

//void PWM::begin(int pin, int freq) {
//  pinMode(pin, OUTPUT);
//  analogWrite(pin, freq);
//  isBegin = true;
//  return;
//}

void PWM::begin() {

  //PWM
  pinMode(motorPwmFreqPin, OUTPUT);
  analogWrite(motorPwmFreqPin, motorPwmFreqVal);

  //方向
  pinMode(motorPwmDirectPin, OUTPUT);
  digitalWrite(motorPwmDirectPin, motorPwmDirectVal);
  
  isBegin = true;
  return;
}
