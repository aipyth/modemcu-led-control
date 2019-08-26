#include <Arduino.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// Redefine these values for your strip and out pin
#define NUM_LEDS 190
#define DATA_PIN 15


struct RGBScheme {
    byte red = 0;
    byte green = 0;
    byte blue = 0;
};
struct HSVScheme {
    byte hue = 0;
    byte saturation = 0;
    byte value = 0;
};

byte brightness = 50;
// colorsheme:
//     0 - RBG
//     1 - HSV
byte colorsheme = 1;
RGBScheme rgb_scheme;
HSVScheme hsv_scheme;

// Effects settings
int mass = 40;
int comets_length = 18;


// Define leds
CRGBArray<NUM_LEDS> leds;


void updateBrightness();


void setupStrip() {
    // FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    updateBrightness();
}

void updateBrightness() {
    FastLED.setBrightness(brightness);
}

void setBrightness(byte brnss) {
    brightness = brnss;
    updateBrightness();
}

void turnOff() {
    FastLED.clear();
    FastLED.show();
}

void wholeColor() {
    leds.fill_solid(colorsheme ? CHSV(hsv_scheme.hue, hsv_scheme.saturation, hsv_scheme.value) : CRGB(rgb_scheme.red, rgb_scheme.green, rgb_scheme.blue) );
    FastLED.show();
}

void rainbow() {
    static uint8_t hue=0;
    leds.fill_rainbow(hue++);
    FastLED.show();
    delay(mass);
}

void comets() {
    static int dot = 0;
    byte fade_value;
    // byte fade_scaled;

    if (comets_length > NUM_LEDS / 2)
        comets_length = NUM_LEDS / 2;

    for (int counter = dot; counter > dot - comets_length; counter--) {
        if (counter >= 0) {
            leds[counter] = colorsheme ? CHSV(hsv_scheme.hue, hsv_scheme.saturation, hsv_scheme.value) : CRGB(rgb_scheme.red, rgb_scheme.green, rgb_scheme.blue);

            fade_value = (256 / comets_length) * (dot - counter);
            fade_value = fade_value + (255 - fade_value) * (random8() % 2) * (random8() % 2);
            // fade_scaled = scale8(fade_value, sizeof(int));

            // leds[counter].fadeToBlackBy(fade_scaled);
            leds[counter].fadeToBlackBy(fade_value);
        }
    }
    FastLED.show();

    dot++;
    if (dot >= NUM_LEDS)
        dot = 0;
        FastLED.clear();
    delay(mass);
}