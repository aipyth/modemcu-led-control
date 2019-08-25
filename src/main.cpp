#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>


// Custom includes
#include "server_response.cpp"
#include "effects.cpp"


// LED settings
byte brightness = 50;
byte red = 30, green = 0, blue = 255;
byte effect = 0;

// WiFi settings
const char ssid[] = "TPL";
const char pass[] = "13096472";

// SoftAP settings
bool soft_ap = false;
const char soft_ssid[] = "Mesh";
const char soft_pass[] = "mesh";

// Init server
ESP8266WebServer server(80);

// Prototypes
void startSoftAP();
void startArduinoOTA();
void runFuckingLights();
void handleRoot();
void handleTurnOff();
void handleComets();
void handleWholeColor();


// Run on setup/load
void setup() {
  Serial.begin(115200);
  Serial.println();
  
  WiFi.begin(ssid, pass);
  Serial.print("Connection to ");
  Serial.print(ssid);
  int connection_time_conter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    connection_time_conter++;
    if (connection_time_conter > 60) {
      Serial.println();
      Serial.print("Cannot connect to the ");
      Serial.println(ssid);
      while (true) {;}
    }
  }
  Serial.print("\nConnected. IP - ");
  Serial.print(WiFi.localIP());
  Serial.println();

  if (soft_ap) {
    startSoftAP();
  }


  server.on("/", handleRoot);
  server.on("/turnoff/", handleTurnOff);
  server.on("/whole/", handleWholeColor);
  server.on("/comets/", handleComets);

  server.begin();
  Serial.println("HTTP server started");

  setupStrip();
  Serial.println("LED Strip setup successful");

  startArduinoOTA();
}


// Run in cycle
void loop() {
  // handle ArduinoOTA connection
  ArduinoOTA.handle();
  // Handle server connection
  server.handleClient();
  // Handle lights
  runFuckingLights();
}

void startSoftAP() {
  WiFi.softAP(soft_ssid, soft_pass);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("SoftAP init successful");
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void startArduinoOTA() {
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
 
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}


void runFuckingLights() {
  switch (effect) {
    case 0: {
      turnOff();
      break;
    }
    case 1: {
      wholeColor();
      break;
    }
    case 2: {
      comets();
    }
  }
}


// Handle main page /
void handleRoot() {
  server.send(200, "text/html", main_page(brightness, red, green, blue));
}

// Handle effects
void handleTurnOff() {
  effect = 0;
  server.send(200, "text/html", base() + "<body><a href=\"/\">Come back</a></body>");
}

void handleWholeColor() {
  effect = 1;
  server.send(200, "text/html", base() + "<body><a href=\"/\">Come back</a></body>");
}

void handleComets() {
  effect = 2;
  server.send(200, "text/html", base() + "<body><a href=\"/\">Come back</a></body>");
}