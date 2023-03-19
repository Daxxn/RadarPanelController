#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RadarPanelPinoutREV1.h"
#include "SIC451.h"
#include "SK9822.h"

#define LED_COUNT 20

void HandleAlert()
{
  Serial.println("ALERT - SIC451 has triggered an alert.");
}

SIC451 ledPower = SIC451(&Wire, Ser::SIC, HandleAlert, Digitl::PWRPins::PGOOD_PIN, Digitl::PWRPins::SALERT_PIN, Digitl::PWRPins::PWR_EN_PIN);
SK9822 ledStip = SK9822(LED_COUNT);

uint8_t wxrBtn = 0;
uint8_t terBtn = 0;
uint8_t state = 1;

uint8_t wxrPWM = 0;
uint8_t terPWM = 0;

uint8_t backlight = 0;

uint8_t slider = 0;
uint16_t vout = 0;

bool powerEnable = false;

bool ind = false;

int GetSliderPos()
{
  return (int)round(((double)slider / (double)255) * (double)LED_COUNT);
}

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
  Serial.print(slider);
  Serial.print(" | ");
  Serial.print(GetSliderPos());
  Serial.print(" | ");
  Serial.print(vout);
  Serial.print(" | ");
  Serial.println(ledPower.CurrentStatus().PWR_GOOD);
}

void setup() {
  pinMode(Digitl::PANELPins::WXR_SW_PIN, INPUT_PULLUP);
  pinMode(Digitl::PANELPins::TERR_SW_PIN, INPUT_PULLUP);
  attachInterrupt(Digitl::PANELPins::WXR_SW_PIN, weatherInter, FALLING);
  attachInterrupt(Digitl::PANELPins::TERR_SW_PIN, terrainInter, FALLING);
  pinMode(Digitl::PANELPins::WXR_PIN, OUTPUT);
  pinMode(Digitl::PANELPins::TERR_PIN, OUTPUT);
  pinMode(Digitl::PANELPins::BACKLIGHT_PIN, OUTPUT);

  Serial.begin(115200);
  Wire.begin();
  SPI.begin();
  ledStip.Begin();

  ledPower.EnablePower(true);

  Serial.println("Startup Complete");
}

void loop() {
  slider = analogRead(Anlg::SLIDERPins::IN_PIN);
  
  ledStip.Clear();
  ledStip.SetRGB(GetSliderPos(), 100,100,100);
  ledStip.SendLEDs();

  ind = !ind;
  digitalWrite(Digitl::PANELPins::STATUS_PIN, ind);
  Serial.println("Getting status");
  // ledPower.GetStatus();
  ledPower.GetVOUT_TEST();
  Serial.println("Status check complete");
  // vout = ledPower.GetVOUT_TEST();

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
  
  // analogWrite(Digitl::PANELPins::WXR_PIN, wxrPWM);
  // analogWrite(Digitl::PANELPins::TERR_PIN, terPWM);

  // state = !state;

  // // digitalWrite(WXR_IND_PIN, state);
  // // digitalWrite(TERR_IND_PIN, !state);

  // backlight++;
  // analogWrite(Digitl::PANELPins::BACKLIGHT_PIN, backlight);

  printDebug();

  delay(200);
}