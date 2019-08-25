#include <Arduino.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 190
#define DATA_PIN 15

CRGB leds[NUM_LEDS];

void setupStrip() {
    // FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
}

void turnOff() {
    FastLED.clear();
    FastLED.show();
}

void wholeColor() {
    fill_solid( &(leds[0]), NUM_LEDS, 0x9966CC );
    FastLED.show();
}

void comets() {
    static int dot = 0;
    int fade_value;
    int length = 10;

    for (int counter = dot; counter > dot - length; counter--) {
        if (counter >= 0) {
            leds[counter] = 0x9966CC;
            fade_value = (256 / length) * (dot - counter);
            leds[counter].fadeToBlackBy(fade_value);
        }
    }
    FastLED.show();

    dot++;
    if (dot >= NUM_LEDS)
        dot = 0;
        FastLED.clear();
    delay(80);
}