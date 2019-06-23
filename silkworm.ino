
#include "FS.h"
#include "Comm.h"
#include "JsonConfig.h"
#include "WebServer.h"
#include "PWM.h"

//#ifdef DEBUG_ESP_PORT
//#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
//#else
//#define DEBUG_MSG(...)
//#endif

//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

Config config;

JsonConfig jsonConfig;
WebServer webServer;
PWM pwmMotor;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(9600);

  //初始化Config
  jsonConfig.initConfiguration(config);
  
  jsonConfig.loadConfiguration(config);
  
  webServer.begin();

  //pwmMotor.begin(config.motorPwmFreqPin, config.motorPwmFreqVal);
  pwmMotor.motorPwmFreqPin = config.motorPwmFreqPin;
  pwmMotor.motorPwmFreqVal = config.motorPwmFreqVal;
  pwmMotor.motorPwmDirectVal = config.motorPwmDirectVal;
  pwmMotor.motorPwmDirectPin = config.motorPwmDirectPin;
  pwmMotor.begin();

  digitalWrite(LED_BUILTIN, LOW);
}

void loop(void) {
  
  webServer.handleClient();   //处理客户端请求

}
