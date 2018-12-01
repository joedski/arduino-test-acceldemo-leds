// Basic demo for accelerometer readings from Adafruit LIS3DH
// Adapted for use with 6 neopixels arranged roughly at the vertices of an octohedron.

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <FastLED.h>

//// Used for software SPI
//#define LIS3DH_CLK 13
//#define LIS3DH_MISO 12
//#define LIS3DH_MOSI 11
//// Used for hardware & software SPI
//#define LIS3DH_CS 10

// software SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define LED_PIN 6
#define LED_COUNT 6

CRGB leds[LED_COUNT];

DEFINE_GRADIENT_PALETTE (heatmap_gp) {
    0,     0,  0,  0,   //black
   64,   255,  0,  0,   //red
  196,   255,255,  0,   //bright yellow
  255,   255,255,255    //full white
};

CRGBPalette16 ledsPalette = heatmap_gp;

CRGB getLedPaletteColor(uint16_t reading) {
  return ColorFromPalette(ledsPalette, reading >> 7);
}


void setup(void) {
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    // Serial.println("Couldnt start");
    // TODO: Show Error LED.
    while (1);
  }

  // Since this is 4G, 1G = 32767 * 1/4 or 32767 >> 2 or 8195
  // By default, the LIS3DH is configured so that at rest, there's 1G in the upwards direction:
  // the 1G counteracting the acceleration due to Earth's gravity.
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  FastLED.setBrightness(64);
}

void loop() {
  // get X Y and Z data at once
  lis.read();
  // x: 2, 4
  // y: 1, 3
  // z: 5, 0
  if (lis.x > 0) {
    leds[4] = getLedPaletteColor(lis.x);
    leds[2] = CRGB::Black;
  }
  else {
    leds[4] = CRGB::Black;
    leds[2] = getLedPaletteColor(-lis.x);
  }

  if (lis.y > 0) {
    leds[3] = getLedPaletteColor(lis.y);
    leds[1] = CRGB::Black;
  }
  else {
    leds[3] = CRGB::Black;
    leds[1] = getLedPaletteColor(-lis.y);
  }

  if (lis.z > 0) {
    leds[5] = getLedPaletteColor(lis.z);
    leds[0] = CRGB::Black;
  }
  else {
    leds[5] = CRGB::Black;
    leds[0] = getLedPaletteColor(-lis.z);
  }

  FastLED.show();

  // TODO: I might want to play with events more.
  // While float math is slower, it's easier to work with,
  // and for vector stuff I'd probably end up having to constantly
  // upgrade those int16_t to int32_t just to deal with overflow.

  // /* Or....get a new sensor event, normalized */
  // sensors_event_t event;
  // lis.getEvent(&event);
  //
  // /* Display the results (acceleration is measured in m/s^2) */
  // Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  // Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  // Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  // Serial.println(" m/s^2 ");
  //
  // Serial.println();
  //
  // delay(200);
}
