#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


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

const char soft_ssid[] = "Mesh";
const char soft_pass[] = "mesh";

ESP8266WebServer server(80);


// Prototypes
void runFuckingLights();
void handleRoot();
void handleTurnOff();
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
    delay(250);
    connection_time_conter++;
    if (connection_time_conter > 60) {
      Serial.print("\n");
      Serial.print("Cannot connect to the ");
      Serial.print(ssid);
      while (true) {;}
    }
  }
  Serial.print("\nConnected. IP - ");
  Serial.print(WiFi.localIP());
  Serial.println();

  WiFi.softAP(soft_ssid, soft_pass);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);


  server.on("/", handleRoot);
  server.on("/turnoff", handleTurnOff);
  server.on("/whole", handleWholeColor);

  server.begin();
  Serial.println("HTTP server started");
  setupStrip();
  Serial.println("LED Strip setup successful");
}


// Run in cycle
void loop() {
  server.handleClient();
  runFuckingLights();
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