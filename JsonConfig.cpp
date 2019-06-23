
#include <ESP8266WiFi.h>
#include "JsonConfig.h"
//extern Config config;

JsonConfig::JsonConfig() {

}

JsonConfig::~JsonConfig() {
  
}

//初始化config
void JsonConfig::initConfiguration(Config &config) {
  strlcpy(config.version, "0.0.1", sizeof(config.version));
  strlcpy(config.hostname, "localhost", sizeof(config.hostname)); 
  strlcpy(config.chip, "ESP8266EXt", sizeof(config.chip)); 

//  strlcpy(config.mac, "none", sizeof(config.mac)); 
  byte mac[6];
  WiFi.macAddress(mac);
  String strMac = String(mac[0],HEX) + ":" + String(mac[1],HEX) + ":" + String(mac[2],HEX) + ":" + String(mac[3],HEX) + ":" + String(mac[4],HEX) + ":" + String(mac[5],HEX);
  strMac.toCharArray(config.mac, sizeof(config.mac));

  config.port = 80;
  config.motorPwmFreq = 1024 * 0.4;
  config.motorPwmPin = D1;

  config.motorPwmDirection = 0;
  config.motorPwmDirectionPin = D2;
  
  return;
}

//加载config文件
void JsonConfig::loadConfiguration(Config &config) {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  //获取参数文件
  if (!SPIFFS.exists(configFilename)) {
    initConfiguration(config);
    saveConfiguration(config);
  }

    
  File file = SPIFFS.open(configFilename, "r");

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
  }

  if (doc.containsKey("version")) {
    strlcpy(config.version, doc["version"], sizeof(config.version));
  }
  
  if (doc.containsKey("hostname")) {
    strlcpy(config.hostname, doc["hostname"], sizeof(config.hostname));
  }

  if (doc.containsKey("chip")) {
    strlcpy(config.chip, doc["chip"], sizeof(config.chip));
  }
  
  if (doc.containsKey("port")) {
    config.port = doc["port"];
  }
  
  if (doc.containsKey("mac")) {
    strlcpy(config.mac, doc["mac"], sizeof(config.mac));
  }

  if (doc.containsKey("motorPwmFreq")) {
    config.motorPwmFreq = doc["motorPwmFreq"];
  }
  
  if (doc.containsKey("motorPwmPin")) {
    config.motorPwmPin = doc["motorPwmPin"];
  }

  if (doc.containsKey("motorPwmDirection")) {
    config.motorPwmDirection = doc["motorPwmDirection"];
  }

  if (doc.containsKey("motorPwmDirectionPin")) {
    config.motorPwmDirectionPin = doc["motorPwmDirectionPin"];
  }

  file.close();
  return;
}

//保存config文件
void JsonConfig::saveConfiguration(Config &config) {
  SPIFFS.remove(configFilename);

  File file = SPIFFS.open(configFilename, "w"); 
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<256> doc;

  doc["version"] = config.version;
  doc["hostname"] = config.hostname;
  doc["chip"] = config.chip;
  doc["mac"] = config.mac;
  doc["port"] = config.port;
  doc["motorPwmFreq"] = config.motorPwmFreq;
  doc["motorPwmPin"] = config.motorPwmPin;
  doc["motorPwmDirection"] = config.motorPwmDirection;
  doc["motorPwmDirectionPin"] = config.motorPwmDirectionPin;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  } else {
    Serial.println(F("Success to write to file"));
  }

  // Close the file
  file.close();

  return;
}

//读取config文件
String JsonConfig::readConfiguration(Config &config) {
  String configJson = "";

  StaticJsonDocument<256> doc;

  doc["version"] = config.version;
  doc["hostname"] = config.hostname;
  doc["chip"] = config.chip;
  doc["mac"] = config.mac;
  doc["port"] = config.port;
  doc["motorPwmFreq"] = config.motorPwmFreq;
  doc["motorPwmPin"] = config.motorPwmPin;

  if (serializeJson(doc, configJson) == 0) {
    Serial.println(F("Failed to write to String"));
  }

  return configJson;
}

////读取config文件
//void JsonConfig::serialConfiguration(Config &config) {
//
//  StaticJsonDocument<256> doc;
//
//  doc["version"] = config.version;
//  doc["hostname"] = config.hostname;
//  doc["chip"] = config.chip;
//  doc["mac"] = config.mac;
//  doc["port"] = config.port;
//  doc["motorPwmFreq"] = config.motorPwmFreq;
//  doc["motorPwmPin"] = config.motorPwmPin;
//
//  if (serializeJson(doc, Serial) == 0) {
//    Serial.println(F("Failed to write to String"));
//  }
//  Serial.println();
//  return ;
//}


// Prints the content of a file to the Serial
void JsonConfig::printFile() {
  // Open file for reading
  File file = SPIFFS.open(configFilename, "r");

  
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();

  return;
}
