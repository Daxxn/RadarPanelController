#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RadarPanelPinoutREV1.h"
#include "SIC451.h"
#include "SK9822.h"

#define LED_COUNT 64

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

int slider = 0;
uint8_t sliderClamp = 0;
uint16_t vout = 0;

bool powerEnable = false;
uint8_t powerCount = 0;
uint8_t maxPowerCount = 10;

bool switchState = false;
bool currentState = false;
bool enable = false;

bool ind = false;

int GetSliderPos()
{
  return (int)round(((double)slider / (double)1023) * (double)LED_COUNT);
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
  Serial.println(GetSliderPos());
  // Serial.print(" | ");
  // Serial.print(vout);
  // Serial.print(" | ");
  // Serial.println(ledPower.CurrentStatus().PWR_GOOD);
}

void InitPins()
{
  pinMode(Digitl::IndicatorPins::ENABLE_SW_PIN, INPUT_PULLUP);
  pinMode(Digitl::PANELPins::WXR_SW_PIN, INPUT_PULLUP);
  pinMode(Digitl::PANELPins::TERR_SW_PIN, INPUT_PULLUP);
  attachInterrupt(Digitl::PANELPins::WXR_SW_PIN, weatherInter, FALLING);
  attachInterrupt(Digitl::PANELPins::TERR_SW_PIN, terrainInter, FALLING);
  // pinMode(Digitl::PANELPins::WXR_IND_PIN, OUTPUT);
  // pinMode(Digitl::PANELPins::TERR_IND_PIN, OUTPUT);
  pinMode(Digitl::PANELPins::BACKLIGHT_PIN, OUTPUT);
}

void setup()
{

  Serial.begin(115200);
  Wire.begin();
  SPI.begin();
  ledStip.Begin();

  ledStip.SetGlobalBrightness(31);

  powerEnable = true;
  ledPower.EnablePower(powerEnable);

  Serial.println("Startup Complete");
}

void loop()
{
  // currentState = digitalRead(Digitl::IndicatorPins::ENABLE_SW_PIN);
  // if (currentState != switchState)
  // {
  //   enable = !enable;
  //   switchState = currentState;
  // }
  // powerCount++;
  // if (powerCount > maxPowerCount)
  // {
  //   powerEnable = !powerEnable;
  //   ledPower.EnablePower(powerEnable);
  //   powerCount = 0;
  // }
  slider = analogRead(Anlg::SLIDERPins::IN_PIN);
  
  // ledStip.Clear();
  // for (size_t i = 0; i < GetSliderPos(); i++)
  // {
  //   ledStip.SetRGB(i, 255, 255, 255);
  // }
  
  // ledStip.SetRGB(GetSliderPos(), 255,255,255);
  // if (enable)
  // {
  //   ledStip.SetGlobalBrightness(31);
  // }
  // else
  // {
  //   ledStip.SetGlobalBrightness(1);
  // }
  // digitalWrite(Digitl::IndicatorPins::STATUS_IND_PIN, enable);
  // delay(1);
  // ledStip.SendLEDs();

  sliderClamp = slider / 4;
  analogWrite(Digitl::PANELPins::TERR_IND_PIN, terPWM);
  analogWrite(Digitl::PANELPins::WXR_IND_PIN, wxrPWM);

  analogWrite(Digitl::PANELPins::BACKLIGHT_PIN, sliderClamp);

  analogWrite(Digitl::PANELPins::LIGHTS_EN_PIN, sliderClamp);

  terPWM = sliderClamp;
  wxrPWM = sliderClamp;

  // ind = !ind;
  // ledPower.EnablePower(ind);
  // Serial.println("Getting Voltage:");
  // ledPower.GetVOUT();
  // Serial.println(ledPower.GetVOUT());
  // ledPower.GetStatus();
  // ledPower.GetVOUT_TEST();
  // Serial.println("Status check complete");
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

  // printDebug();

  delay(200);
}