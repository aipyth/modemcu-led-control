#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 190
#define DATA_PIN 15

CRGB leds[NUM_LEDS];

void setupStrip() {
    // FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
}

void wholeColor() {
    static int dot = 0;
    leds[dot] = CRGB::FireBrick;
    FastLED.show();
    for (int counter = dot; counter > dot - 8; counter--) {
        if (counter >= 0)
            leds[counter].nscale8(32 + 28 * counter);
    }
    // leds[dot].nscale8(192);
    // leds[dot - 1].nscale8(192 - 32);
    // leds[dot - 2].nscale8(192 - 64);
    // leds[dot - 3].nscale8(192 - 96);
    dot++;
    if (dot >= NUM_LEDS)
        dot = 0;
        FastLED.clear();
    delay(100);
}

void turnOff() {
    FastLED.clear();
    FastLED.show();
}