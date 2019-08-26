#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
// #include "effects.cpp"


extern ESP8266WebServer server;

// LED settings
extern int effect;

extern byte brightness;
extern byte colorsheme;

extern RGBScheme rgb_scheme;
extern HSVScheme hsv_scheme;

extern int mass;
extern int comets_length;


template <typename C>
void update_key_if_exists(DynamicJsonDocument &doc, const char* name, C &key) {
    if (doc.containsKey(name)) {
        if (doc[name].is<C>()) {
            C value = doc[name];
            key = value;
            
            Serial.print(name);
            Serial.print(" set via API. Value = ");
            Serial.println(value);
        }
    }
}

//   /api/
void handleAPIRoot() {
    String available_methods = String("/SKV Structure/");
    server.send(200, "application/json", available_methods);
}

//   /api/basic/
void handleAPIBasic() {
    /*
        {
            "brightness":      0-255,
            "scheme":          0-1,
            "mass":            0-16192,
            "comets_length":   0-(NUM_LEDS / 2),
            "effect":          0-(number of effects)
        }
    */
    if (server.hasArg("plain")) {
        String raw_json = server.arg("plain");
        // Serial.println(raw_json);
        const int BUFFER_SIZE = JSON_OBJECT_SIZE(5) + 64;
        DynamicJsonDocument doc(BUFFER_SIZE);
        // DynamicJsonBuffer doc;
        DeserializationError err = deserializeJson(doc, raw_json);

        if (err) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(err.c_str());

            // Serial.println("Error deserializing json data");
            server.send(505, "text/plain", "Deserialization error");
            return;
        }

        // brightness
        update_key_if_exists<byte>(doc, "brightness", brightness);
        updateBrightness();
        // scheme
        update_key_if_exists<byte>(doc, "scheme", colorsheme);
        // mass
        update_key_if_exists<int>(doc, "mass", mass);
        // comets length
        update_key_if_exists<int>(doc, "comets_length", comets_length);
        // effect
        update_key_if_exists<int>(doc, "effect", effect);



        server.send(200, "text/plain", "{\"state\": \"ok\"}");
        return;
    }
    server.send(404);
}

void handleSetRGB() {
    /*
        {
            "red": 0-255,
            "green": 0-255,
            "blue": 0-255
        }
    */
    if (server.hasArg("plain")) {
        String raw_json = server.arg("plain");

        const int BUFFER_SIZE = JSON_OBJECT_SIZE(3) + 64;
        DynamicJsonDocument doc(BUFFER_SIZE);
        DeserializationError err = deserializeJson(doc, raw_json);

        if (err) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(err.c_str());

            server.send(505, "text/plain", "Deserialization error");
            return;
        }

        update_key_if_exists<byte>(doc, "red", rgb_scheme.red);
        update_key_if_exists<byte>(doc, "green", rgb_scheme.green);
        update_key_if_exists<byte>(doc, "blue", rgb_scheme.blue);
        
        server.send(200, "text/plain", "{\"state\": \"ok\"}");
        return;
    }
    server.send(404);
}

void handleSetHSV() {
    /*
        {
            "hue": 0-255,
            "saturation": 0-255,
            "value": 0-255
        }
    */
    if (server.hasArg("plain")) {
        String raw_json = server.arg("plain");

        const int BUFFER_SIZE = JSON_OBJECT_SIZE(3) + 64;
        DynamicJsonDocument doc(BUFFER_SIZE);
        DeserializationError err = deserializeJson(doc, raw_json);

        if (err) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(err.c_str());

            server.send(505, "text/plain", "Deserialization error");
            return;
        }

        update_key_if_exists<byte>(doc, "hue", hsv_scheme.hue);
        update_key_if_exists<byte>(doc, "saturation", hsv_scheme.saturation);
        update_key_if_exists<byte>(doc, "value", hsv_scheme.value);
        
        server.send(200, "text/plain", "{\"state\": \"ok\"}");
        return;
    }
    server.send(404);
}
