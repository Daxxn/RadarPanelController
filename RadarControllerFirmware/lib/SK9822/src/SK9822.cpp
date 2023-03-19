#include <Arduino.h>
#include "SK9822.h"

SK9822::SK9822(int ledCount)
{
  _ledCount = ledCount;
  _leds = new LED[ledCount];
}

SK9822::~SK9822()
{
  delete[] _leds;
}

void SK9822::Begin()
{
  Clear();
}

void SK9822::Clear()
{
  for (size_t i = 0; i < _ledCount; i++)
  {
    _leds[i].Black();
  }
}

void SK9822::SetBrightness(uint8_t bright)
{
  _globalBright = bright;
}

void SK9822::SetRGB(int index, uint8_t red, uint8_t green, uint8_t blue)
{
  _leds[index].SetRGB(red, green, blue);
}

void SK9822::SetAllRGB(uint8_t red, uint8_t green, uint8_t blue)
{
  for (size_t i = 0; i < _ledCount; i++)
  {
    _leds[i].SetRGB(red, green, blue);
  }
}

void SK9822::SetAllRGB(uint8_t bright, uint8_t red, uint8_t green, uint8_t blue)
{
  SetBrightness(bright);
  SetAllRGB(red, green, blue);
}

void SK9822::White()
{
  for (size_t i = 0; i < _ledCount; i++)
  {
    _leds[i].White();
  }
  
}

LED SK9822::GetLED(int index)
{
  return _leds[index];
}

void SK9822::SendLEDs()
{
  SPI.beginTransaction(SK9822Models::Constants::SETTINGS);

  // Send the start packet.
  // union { uint32_t value; struct { uint16_t top; uint16_t bot; }; } start, end;
  // start.value = SK9822Models::Constants::START_FRAME;
  // end.value = SK9822Models::Constants::END_FRAME;

  // _spi->transfer16(start.top);
  // _spi->transfer16(start.bot);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  for (size_t i = 0; i < _ledCount; i++)
  {
    // Send the global brightness first.
    SPI.transfer(0xE0 | _globalBright);
    SPI.transfer(_leds[i].blue);
    SPI.transfer(_leds[i].green);
    SPI.transfer(_leds[i].red);
  }
  
  // Send the end packet.
  // _spi->transfer16(end.top);
  // _spi->transfer16(end.bot);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);

  SPI.endTransaction();
}