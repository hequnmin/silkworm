#include "arduino.h"

#ifndef _Comm_H__
#define _Comm_H__

enum Component { motor, thermostat };

struct Config {
  char version[10];
  char hostname[64];
  uint8_t port;
  char chip[64];
  char mac[64];
  char ssid[64];
  char psk[64];

  int motorPwmFreqVal;          //default: 1024*0.4, min value: 0, max value: 1024
  int motorPwmFreqPin;          //default: D1
  int motorPwmDirectVal;        //default: 0
  int motorPwmDirectPin;        //default: D2

};

struct Status {
  bool motorHandling;
};



#endif
