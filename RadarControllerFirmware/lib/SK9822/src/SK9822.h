#pragma once

#include <Arduino.h>
#include <SPI.h>

namespace SK9822Models
{
  namespace Constants
  {
    const uint32_t START_FRAME = 0;
    const uint32_t END_FRAME = 0xFFFFFFFF;
    const SPISettings SETTINGS(100000, MSBFIRST, SPI_MODE0);
  } // namespace Constants
  
} // namespace SK9822Models

struct LED
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  void Black()
  {
    red = 0;
    green = 0;
    blue = 0;
  }
  void White()
  {
    red = 255;
    green = 255;
    blue = 255;
  }
  void SetRGB(uint8_t red, uint8_t green, uint8_t blue)
  {
    this->red = red;
    this->green = green;
    this->blue = blue;
  }
};

class SK9822
{
public:
  SK9822(int ledCount);
  ~SK9822();

  void Begin();
  void Clear();

  void SetGlobalBrightness(uint8_t brgt);

  void SendLEDs();

  void SetRGB(int index, uint8_t red, uint8_t green, uint8_t blue);
  void White();

  void SetAllRGB(uint8_t red, uint8_t green, uint8_t blue);
  void SetAllRGB(uint8_t bright, uint8_t red, uint8_t green, uint8_t blue);

  LED GetLED(int index);

private:
  unsigned int _ledCount;
  LED* _leds;
  uint8_t _globalBright;
};