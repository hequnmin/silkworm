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
    
    void begin(int pin, int freq);
};

#endif
