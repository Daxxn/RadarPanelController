#include <Arduino.h>
#include "FastLED.h"

#define LED_COUNT 20

const int LED_DATA_PIN = 8;

const int WEATHER_BTN_PIN = 0;
const int TERR_BTN_PIN = 1;

const int WXR_TEST_PIN = 3;
const int TER_TEST_PIN = 4;

const int BACKLIGHT_PIN = 6;
const int WXR_IND_PIN = 9;
const int TERR_IND_PIN = 10;

CRGB leds[LED_COUNT];

uint8_t wxrBtn = 0;
uint8_t terBtn = 0;
uint8_t state = 1;

uint8_t wxrPWM = 0;
uint8_t terPWM = 0;

uint8_t backlight = 0;

void weatherInter()
{
  Serial.println("WXR");
  wxrPWM += 2;
}

void terrainInter()
{
  Serial.println("TER");
  terPWM += 2;
}

void printDebug()
{
  Serial.print(state);
  Serial.print(" | ");
  Serial.print(backlight);
  Serial.print(" | ");
  Serial.print(wxrBtn);
  Serial.print(" | ");
  Serial.print(terBtn);
  Serial.print(" | ");
  Serial.print(wxrPWM);
  Serial.print(" | ");
  Serial.println(terPWM);
}

void setup() {
  pinMode(WXR_TEST_PIN, INPUT_PULLUP);
  pinMode(TER_TEST_PIN, INPUT_PULLUP);
  pinMode(INT2, INPUT_PULLUP);
  pinMode(INT3, INPUT_PULLUP);
  attachInterrupt(INT2, weatherInter, FALLING);
  attachInterrupt(INT3, terrainInter, FALLING);
  // pinMode(WXR_IND_PIN, OUTPUT);
  // pinMode(TERR_IND_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // wxrBtn = digitalRead(WXR_TEST_PIN);
  // terBtn = digitalRead(TER_TEST_PIN);

  // if (wxrBtn == 0)
  // {
  //   wxrPWM += 10;
  // }
  // if (terBtn == 0)
  // {
  //   terPWM += 10;
  // }
  
  analogWrite(WXR_IND_PIN, wxrPWM);
  analogWrite(TERR_IND_PIN, terPWM);

  state = !state;

  // digitalWrite(WXR_IND_PIN, state);
  // digitalWrite(TERR_IND_PIN, !state);

  backlight++;
  analogWrite(BACKLIGHT_PIN, backlight);

  printDebug();
  delay(500);
}