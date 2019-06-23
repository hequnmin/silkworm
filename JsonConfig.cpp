
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
  config.motorPwmFreqVal = 1024 * 0.4;
  config.motorPwmFreqPin = D1;

  config.motorPwmDirectVal = 0;
  config.motorPwmDirectPin = D2;
  
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

  if (doc.containsKey("motorPwmFreqVal")) {
    config.motorPwmFreqVal = doc["motorPwmFreqVal"];
  }
  
  if (doc.containsKey("motorPwmFreqPin")) {
    config.motorPwmFreqPin = doc["motorPwmFreqPin"];
  }

  if (doc.containsKey("motorPwmDirectVal")) {
    config.motorPwmDirectVal = doc["motorPwmDirectVal"];
  }

  if (doc.containsKey("motorPwmDirectPin")) {
    config.motorPwmDirectPin = doc["motorPwmDirectPin"];
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
  doc["motorPwmFreqVal"] = config.motorPwmFreqVal;
  doc["motorPwmFreqPin"] = config.motorPwmFreqPin;
  doc["motorPwmDirectVal"] = config.motorPwmDirectVal;
  doc["motorPwmDirectPin"] = config.motorPwmDirectPin;

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
String JsonConfig::readConfiguration() {
    String configJson = "";
    if (SPIFFS.exists(configFilename)) {
      File file = SPIFFS.open(configFilename, "r");
      if (file) {
        configJson = file.readString();
      }
      file.close();
    }
    return configJson;

}

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
