// Pulse Width Modulation
#include "PWM.h"

PWM::PWM() {

}

PWM::~PWM() {
  
}

void PWM::begin(int pin, int freq) {
  pinMode(pin, OUTPUT);
  analogWrite(pin, freq);
  isBegin = true;
  return;
}
