#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
// #include <ArduinoJson.h>

// Custom includes
#include "server_response.cpp"
#include "effects.cpp"
#include "api.cpp"


// LED settings
int effect = 0;

// extern byte brightness;
// extern byte colorsheme;

// extern RGBScheme rgb_scheme;
// extern HSVScheme hsv_scheme;

// extern int mass;
// extern int comets_length;

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
void handleRainbow();
void handleWholeColor();
void handleComets();


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
      startSoftAP();
      while (true) {;}
    }
  }
  Serial.print("\nConnected. IP - ");
  Serial.print(WiFi.localIP());
  Serial.println();

  if (soft_ap) {
    startSoftAP();
  }


  // Handle standart http requests
  server.on("/", handleRoot);
  server.on("/turnoff/", handleTurnOff);
  server.on("/whole/", handleWholeColor);
  server.on("/rainbow/", handleRainbow);
  server.on("/comets/", handleComets);

  // Handle API requests
  server.on("/api/", handleAPIRoot);
  server.on("/api/basic/", handleAPIBasic);
  server.on("/api/setrgb/", handleSetRGB);
  server.on("/api/sethsv/", handleSetHSV);

  // Start server
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
      rainbow();
      break;
    }
    case 3: {
      comets();
      break;
    }
  }
}


// Handle main page /
void handleRoot() {
  if (server.args() > 0) {
    String message = String();

    for (int i = 0; i < server.args(); i++) {
      String name = server.argName(i);
      String value = server.arg(i);

      message += "Arg nº" + (String)i + " –> ";
      message += name + ": ";
      message += value + "\n";
      Serial.println(message);

      if (name == "brightness") {
        brightness = value.toInt();
        updateBrightness();
      }
      if (name == "mass") {
        mass = value.toInt();
      }
      if (name == "colorsheme") {
        colorsheme = value.toInt();
      }
      if (name == "red") {
        rgb_scheme.red = value.toInt();
      } 
      if (name == "green") {
        rgb_scheme.green = value.toInt();
      }
      if (name == "blue") {
        rgb_scheme.blue = value.toInt();
      }
      if (name == "hue") {
        hsv_scheme.hue = value.toInt();
      }
      if (name == "saturation") {
        hsv_scheme.saturation = value.toInt();
      }
      if (name == "value") {
        hsv_scheme.value = value.toInt();
      }
    }
  }
  server.send(200, "text/html", main_page(brightness, colorsheme, mass, rgb_scheme.red, rgb_scheme.green, rgb_scheme.blue, hsv_scheme.hue, hsv_scheme.saturation, hsv_scheme.value));
}

// Handle effects
void handleTurnOff() {
  effect = 0;
  server.send(200, "text/html", redirect_to_main( WiFi.localIP().toString() ));
}

void handleWholeColor() {
  effect = 1;
  server.send(200, "text/html", redirect_to_main( WiFi.localIP().toString() ));
}

void handleRainbow() {
  effect = 2;
  server.send(200, "text/html", redirect_to_main( WiFi.localIP().toString() ));
}

void handleComets() {
  effect = 3;
  server.send(200, "text/html", redirect_to_main( WiFi.localIP().toString() ));
}