
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#ifndef _WebServer_H__
#define _WebServer_H__

class WebServer {
  private:
    const int maxReconect = 30;
  public:
    WebServer();
    ~WebServer();

    
    void begin();
    
    void handleRoot();
    void handleNotFound();

    void handleClient();

    void returnFail(String msg);

    void handleLogin();

    void handleScanNetwork();
    
    bool isAuth();

    void handleConfig();

    void handleFile();
};

#endif
