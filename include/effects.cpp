#include <Arduino.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// Redefine these values for your strip and out pin
#define NUM_LEDS 120
#define DATA_PIN 15


struct RGBScheme {
    byte red = 0;
    byte green = 0;
    byte blue = 0;
};
struct HSVScheme {
    byte hue = 184;
    byte saturation = 240;
    byte value = 255;
};

byte brightness = 255;
// colorsheme:
//     0 - RBG
//     1 - HSV
byte colorsheme = 1;
RGBScheme rgb_scheme;
HSVScheme hsv_scheme;

// Effects settings
int mass = 20;
int comets_length = 18;
int fire_cooling = 30;
int fire_sparking = 70;

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

void colorWipe() {
    static int step = 0;
    static bool back = false;
    if (back) {
        leds[step] = CRGB(0, 0, 0);
    } else {
        leds[step] = colorsheme ? CHSV(hsv_scheme.hue, hsv_scheme.saturation, hsv_scheme.value) : CRGB(rgb_scheme.red, rgb_scheme.green, rgb_scheme.blue);
    }
    FastLED.show();
    delay(mass);
    step++;
    if (step >= NUM_LEDS) {
        step = 0;
        back = !back;
    }
}

void setPixelHeatColor (int Pixel, byte temperature) {
    // Scale 'heat' down from 0-255 to 0-191
    byte t192 = round((temperature/255.0)*191);
    
    // calculate ramp up from
    byte heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252
    
    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
        leds[Pixel] = CRGB(255, 255, heatramp);
    } else if( t192 > 0x40 ) {             // middle
        leds[Pixel] = CRGB(255, heatramp, 0);
    } else {                               // coolest
        leds[Pixel] = CRGB(heatramp, 0, 0);
    }
}

void fire() {
    static byte heat[NUM_LEDS];
    int cooldown;
    
    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
        cooldown = random(0, ((fire_cooling * 10) / NUM_LEDS) + 2);
        
        if(cooldown>heat[i]) {
            heat[i]=0;
        } else {
            heat[i]=heat[i]-cooldown;
        }
    }
    
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
        
    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if( random(255) < fire_sparking ) {
        int y = random(7);
        heat[y] = heat[y] + random(160,255);
        //heat[y] = random(160,255);
    }

    // Step 4.  Convert heat to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
        setPixelHeatColor(j, heat[j] );
    }
    FastLED.show();
    delay(mass);
}

void runningLights() {
    static int Position=0;
    static int i = 0;
        
    Position++; // = 0; //Position + Rate;
    for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        if (colorsheme) {
            leds[i] = CHSV(hsv_scheme.hue,
                        ((sin(i+Position) * 127 + 128)/255) * hsv_scheme.saturation,
                        ((sin(i+Position) * 127 + 128)/255) * hsv_scheme.value );
        } else {
            leds[i] = CRGB( ((sin(i+Position) * 127 + 128)/255)*rgb_scheme.red,
                            ((sin(i+Position) * 127 + 128)/255)*rgb_scheme.green,
                            ((sin(i+Position) * 127 + 128)/255)*rgb_scheme.blue );
        }
    }
    
    FastLED.show();
    delay(mass);
    if (i >= NUM_LEDS) {
        i = 0;
    }
}