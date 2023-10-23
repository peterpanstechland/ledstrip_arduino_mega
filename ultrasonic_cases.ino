/*
    UltrasonicDisplayOnTerm.ino
    Example sketch for ultrasonic ranger

    Copyright (c) 2012 seeed technology inc.
    Website    : www.seeed.cc
    Author     : LG, FrankieChu
    Create Time: Jan 17,2013
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


/***************************************************************************/
//	Function: Measure the distance to obstacles in front and print the distance
//			  value to the serial terminal.The measured distance is from
//			  the range 0 to 400cm(157 inches).
//	Hardware: Grove - Ultrasonic Ranger
//	Arduino IDE: Arduino-1.0
/*****************************************************************************/

#include "Ultrasonic.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 2
#define NUM_LEDS 50


Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


Ultrasonic ultrasonic(4);

void rainbow(uint8_t wait);

unsigned long previousMillis = 0;  // Variable to store the previous time
unsigned long rbpreviousMillis = 0;
const uint8_t rainbowDelay = 20;     // Delay between color transitions in milliseconds
const unsigned long interval = 500;  // Interval between measurements in milliseconds

enum DistanceCase {
  CASE_GREEN,
  CASE_RED,
  CASE_RAINBOW,
  CASE_FAR
};

DistanceCase currentCase = CASE_FAR;  // Current distance case

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();  // Initialize all LEDs to off
  delay(1000);   // Delay for 1 second

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));  // Red color (R, G, B)
  }

  strip.show();  // Update the LED strip
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current time

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the previous time

    unsigned int distance = ultrasonic.MeasureInCentimeters();  // Get distance in centimeters

    DistanceCase newCase = currentCase;

    if (distance < 15) {
      newCase = CASE_GREEN;
    } else if (distance >= 20 && distance < 40) {
      newCase = CASE_RED;
    } else if (distance >= 40 && distance < 60) {
      newCase = CASE_RAINBOW;
    }

    if (newCase != currentCase) {
      switch (newCase) {
        case CASE_GREEN:
          // // Handle close distance case
          strip.begin();
          strip.show();  // Initialize all LEDs to off
          delay(1000);   // Delay for 1 second

          for (int i = 0; i < NUM_LEDS; i++) {
            strip.setPixelColor(i, strip.Color(0, 255, 0));  // Red color (R, G, B)
          }

          strip.show();  // Update the LED strip
          Serial.println("GREEN!");
          break;

        case CASE_RED:
          // Handle moderate distance case
          strip.begin();
          strip.show();  // Initialize all LEDs to off
          delay(1000);   // Delay for 1 second

          for (int i = 0; i < NUM_LEDS; i++) {
            strip.setPixelColor(i, strip.Color(255, 0, 0));  // Red color (R, G, B)
          }

          strip.show();  // Update the LED strip
          Serial.println("RED!");
          break;

        case CASE_RAINBOW:
          // Handle moderate distance case
          if (millis() - rbpreviousMillis >= rainbowDelay) {
            rbpreviousMillis = millis();  // Update previousMillis

            rainbowCycle(rainbowDelay);  // Call rainbowCycle function with the specified delay
          }
          Serial.println("RAINBOW!");
          break;

        case CASE_FAR:
          // Handle far distance case
          // for (int i = 0; i < NUM_LEDS; i++) {
          //   strip.setPixelColor(i, strip.Color(255, 255, 255));  // Red color (R, G, B)
          // }

          // strip.show();  // Update the LED strip

          Serial.println("Object is far away.");
          break;
      }

      currentCase = newCase;
    }
  }
}

void rainbowCycle(uint8_t wait) {
  static uint16_t cycle = 0;

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    uint32_t color = Wheel(((i * 256 / strip.numPixels()) + cycle) & 255);
    strip.setPixelColor(i, color);
  }

  strip.show();

  cycle++;
  if (cycle >= 256 * 5) {  // 5 cycles of all colors
    cycle = 0;
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
