
#include "Comm.h"
#include "WebServer.h"
#include "JsonConfig.h"
#include "PWM.h"


#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
//#define STASSID "iHome"
//#define STAPSK  "Welcome2Home"
#define STASSID "Blueway-VIP"
#define STAPSK  "blueway456"
#endif

using namespace std;

extern JsonConfig jsonConfig;
extern Config config;
extern PWM pwmMotor;

ESP8266WebServer server(80); 

const char* ssid = STASSID;
const char* password = STAPSK;


WebServer::WebServer() {
  SPIFFS.begin();
}

WebServer::~WebServer() {
  
}

void WebServer::begin() {

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [&]() {
    handleRoot();
  });

  server.on("/login", [&]() { 
    handleLogin();
  });

  server.on("/inline", []() {
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/plain", "this works as well");
    digitalWrite(LED_BUILTIN, LOW);
  });

  server.on("/config", [&]() {
    handleConfig();
  });

  server.on("/file", [&]() {
    handleFile();
  });

  server.onNotFound([&]() {
    handleNotFound();
  });

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  
  server.begin();
  Serial.println("HTTP server started.");  
}

void WebServer::handleRoot() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Enter handleRoot");

  String response = "Not found page.";
//  if (!isAuth()) {
//    server.sendHeader("Location", "/login");
//    server.sendHeader("Cache-Control", "no-cache");
//    server.send(301);
//    
//    return;
//  }

  bool exist = SPIFFS.exists("/dashboard.html");

  String motorPwmFreqTag = "$MOTOR_PWM_FREQ$";
  String motorPwmPinTag = "$MOTOR_PWM_PIN$";
  
  if (exist) {
    Serial.println("/dashboard.html is exists.");
    File file = SPIFFS.open("/dashboard.html", "r");
    String response = file.readString();

    response.replace(motorPwmFreqTag, String(config.motorPwmFreq));
    response.replace(motorPwmPinTag, String(config.motorPwmPin));
    
    server.send(200, "text/html", response);
    file.close();
  } else {
    returnFail("Not found /dashboard.html.");
  }
    
  digitalWrite(LED_BUILTIN, LOW);
}

void WebServer::handleNotFound() {
  digitalWrite(LED_BUILTIN, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, LOW);
}

void WebServer::handleClient() {
  server.handleClient();
  MDNS.update();
}

void WebServer::returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void WebServer::handleLogin() {
  String messageTag = "{MESSAGE}";
  String message = "Please input username and password.";

  bool logined = false;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    String username = server.arg("USERNAME");
    String password = server.arg("PASSWORD");
    Serial.println(username);
    Serial.println(password);
    if (server.arg("USERNAME") == "" || server.arg("PASSWORD") == "") {
      message = "Please input username and password.";
      Serial.println("Log in Failed");
    } else {
      //if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin") {
      if (server.arg("PASSWORD") == "admin") { 
        logined = true;
        server.sendHeader("Location", "/");
        server.sendHeader("Cache-Control", "no-cache");
        server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
        server.send(301);
        Serial.println("Log in Successful");
        return;
      } else {
        message = "Wrong username or password! try again.";
        Serial.println("Log in Failed");
      }
    }

  }

//  String content = "<html><body><form action='/login' method='POST'>To log in, please use : admin/admin<br>";
//  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
//  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
//  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
//  content += "You also can go <a href='/inline'>here</a></body></html>";
//  server.send(200, "text/html", content);



  bool  exist = SPIFFS.exists("/login.html");
  
  if (exist) {
    Serial.println("/login.html is exists.");
    File file = SPIFFS.open("/login.html", "r");
    String response = file.readString();

    response.replace(messageTag, message);
    
    server.send(200, "text/html", response);
    file.close();
  } else {
    returnFail("Not found /login.html.");
    return;
  }
  

  
}

void WebServer::handleScanNetwork() {
  bool exist = SPIFFS.exists("/scan.html");
  if (exist) {
    File file = SPIFFS.open("/scan.html", "r");
    String response = file.readString();

    server.send(200, "text/html", response);
    file.close();
  } else {
    returnFail("Not found /dashboard.html.");
  }
}

bool WebServer::isAuth() {
  Serial.println("Enter isAuthenticated");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentication Successful");
      return true;
    }
  }
  Serial.println("Authentication Failed");
  return false;
}

void WebServer::handleConfig() {
  Serial.println("WebServer::handleConfig...");

  // 接收GET请求
  if (server.method() == HTTP_GET) {
    String response = jsonConfig.readConfiguration(config);
    server.send(200, "text/json", response);
    return;
  }

  // 接收POST请求
  if (server.method() == HTTP_POST) {
    String request = server.arg("plain");

    Serial.println(request);

//    StaticJsonDocument<512> doc;
    const size_t CAPACITY = JSON_OBJECT_SIZE(20);
    StaticJsonDocument<CAPACITY> doc; 
    
    deserializeJson(doc, request);

    JsonObject object = doc.as<JsonObject>();
        
    
    // 设置参数值
    if (object.isNull()) {
      server.send(200, "text/json", "{\"errorcode\":1, \"errorinfo\": \"invalid request.\"}");
      return;      
    } else {
      if (object.containsKey("motorPwmFreq")) config.motorPwmFreq = object["motorPwmFreq"];
      if (object.containsKey("motorPwmPin")) config.motorPwmPin = object["motorPwmPin"];

      jsonConfig.saveConfiguration(config);
      
      pwmMotor.begin(config.motorPwmPin, config.motorPwmFreq);
      
      server.send(200, "text/json", "{\"errorcode\":0}");
      return;    
    }

  }
}

void WebServer::handleFile() {
  Serial.println("WebServer::handleFile...");
  String response;

  // GET请求，显示所有文件
  if (server.method() == HTTP_GET) {
    const size_t CAPACITY = JSON_OBJECT_SIZE(20);
    StaticJsonDocument<CAPACITY> doc; 

    Dir dir = SPIFFS.openDir("");
    while (dir.next()) {
      doc.add(dir.fileName());
    }
    
    JsonArray arrFile = doc.as<JsonArray>();
    for(JsonVariant v : arrFile) {
        Serial.println(v.as<String>());
    }
    serializeJsonPretty(doc, response);
      
    server.send(200, "text/json", response);
    return;
  }

  // POST请求，操作指定文件
  if (server.method() == HTTP_POST) {
    String response = "";
    String request = server.arg("plain");

    Serial.println(request);

    const size_t CAPACITY = JSON_OBJECT_SIZE(20);
    StaticJsonDocument<CAPACITY> docPost; 

    deserializeJson(docPost, request);

    if (docPost.isNull()) {
      server.send(200, "text/json", "{\"errorcode\":1, \"errorinfo\": \"invalid request.\"}");
      return;      
    } else {
      if (docPost.containsKey("remove")) {
        String strFile = docPost["remove"];
        if (!SPIFFS.exists(strFile)) {
          
//          DynamicJsonDocument docResponse(128);
//          docResponse["errorcode"] = 1;
//          docResponse["errorinfo"] = "not exists file.";
//          serializeJson(docResponse, response);

          response = "{\"errorcode\":1,\"errorinfo\":\"not exists file.\"}";
          server.send(200, "text/json", response);
          
          return;
        }
        SPIFFS.remove(strFile);
        Serial.print("SPIFFS.remove:");
        Serial.println(strFile);
      }

      if (docPost.containsKey("open")) {
        String strFile = docPost["open"];
        if (!SPIFFS.exists(strFile)) {
          response = "{\"errorcode\":1,\"errorinfo\":\"not exists file.\"}";
          server.send(200, "text/json", response);
          return;
        }
        File file = SPIFFS.open(strFile, "r");
        if (file) {
          server.send(200, "text/json", file.readString());
          return;
        }
        
      }
      
      server.send(200, "text/json", "{\"errorcode\":0}");
      return;
    }
  }
}
