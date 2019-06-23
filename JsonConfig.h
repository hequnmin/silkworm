
#include "Comm.h"
#include <ArduinoJson.h>
#include "FS.h"

#ifndef _JsonConfig_H__
#define _JsonConfig_H__



class JsonConfig {
  private:
    const char *configFilename = "/config.json";
  public:
    JsonConfig();
    ~JsonConfig();

    void initConfiguration(Config &config);
    void loadConfiguration(Config &config);
    void saveConfiguration(Config &config);
    void printFile();

    String readConfiguration();
    void serialConfiguration(Config &config);
};

#endif
