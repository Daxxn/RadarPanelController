#pragma once

#include <Arduino.h>

uint16_t CombineBytes(uint8_t upper, uint8_t lower)
{
  return (upper << 8) | lower;
}

uint16_t CombineBytes(uint8_t *arr)
{
  return CombineBytes(arr[1], arr[0]);
  // return (arr[1] << 8) | arr[0];
}