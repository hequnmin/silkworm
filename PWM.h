// Pulse Width Modulation
#include <arduino.h>

#ifndef _PWM_H__
#define _PWM_H__

class PWM {
  private:

    
  public:
    PWM();
    ~PWM();
    bool isBegin = false;

    int motorPwmFreqPin = D1;
    int motorPwmFreqVal = 1024 * 0.4;
    int motorPwmDirectVal = 0;
    int motorPwmDirectPin = D2;
    
//    void begin(int pin, int freq);
    void begin();
};

#endif
