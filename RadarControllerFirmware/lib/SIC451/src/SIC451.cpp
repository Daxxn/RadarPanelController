#include <Arduino.h>
#include <Wire.h>
#include "SIC451.h"

SIC451::SIC451(TwoWire* wire, uint8_t address, void (*interrupt)(void), int pGoodPin, int sAlertPin, int enablePin)
{
  this->_wire = wire;
  this->_interrupt = interrupt;
  this->_address = address;
  this->_pGoodPin = pGoodPin;
  this->_sAlertPin = sAlertPin;
  this->_enablePin = enablePin;

  pinMode(pGoodPin, INPUT);
  pinMode(sAlertPin, INPUT);
  pinMode(enablePin, OUTPUT);

  attachInterrupt(sAlertPin, interrupt, RISING);

  this->_writeAddr = address;
  this->_readAddr = address + 1;
}

SIC451::SIC451(TwoWire* wire, uint8_t address, int pGoodPin, int sAlertPin, int enablePin)
{
  this->_wire = wire;
  this->_address = address;
  this->_pGoodPin = pGoodPin;
  this->_sAlertPin = sAlertPin;
  this->_enablePin = enablePin;

  pinMode(pGoodPin, INPUT);
  pinMode(sAlertPin, INPUT);
  pinMode(enablePin, OUTPUT);

  this->_writeAddr = address;
  this->_readAddr = address + 1;
}

#pragma region PMBus Functions
void SIC451::GetStatus()
{
  Serial.println("Init word union");
  tempWord.word = 0;
  Serial.println("Entering ReadCommand()");
  this->ReadCommand(SICConsts::SICRegisters::STATUS_WORD);
  Serial.println("Parsing status");
  ParseStatus(tempWord.word);
  Serial.println("Status parse complete.");
}
#pragma endregion

#pragma region Pin States
bool SIC451::GetPGood()
{
  this->_pGood = (bool)digitalRead(this->_pGoodPin);
  return this->_pGood;
}

bool SIC451::GetAlert()
{
  this->_alert = (bool)digitalRead(this->_sAlertPin);
  return this->_alert;
}

void SIC451::EnablePower()
{
  this->_en = true;
  digitalWrite(this->_enablePin, this->_en);
}

void SIC451::EnablePower(bool en)
{
  this->_en = en;
  digitalWrite(this->_enablePin, en);
}

bool SIC451::CheckAlert()
{
  this->_alert = digitalRead(this->_sAlertPin);
  return this->_alert;
}
#pragma endregion

#pragma region Internal PMBus Comms
void SIC451::WriteCommand(uint8_t command, uint8_t data)
{
  this->_wire->beginTransmission(this->_writeAddr);
  this->_wire->write(command);
  this->_wire->write(data);
  this->_wire->endTransmission();
}

void SIC451::ReadCommand(uint8_t command, uint8_t* buffer, uint8_t len)
{
  this->_wire->beginTransmission(this->_readAddr);
  this->_wire->write(command);
  this->_wire->endTransmission();
  this->_wire->requestFrom(this->_readAddr, len);
  this->_wire->readBytes(buffer, len);
}

void SIC451::ReadCommand(uint8_t command)
{
  Serial.println("Sending Command");

  this->_wire->requestFrom(this->_readAddr, (uint8_t)2);
  this->_wire->write(command);
  uint8_t wordTop = this->_wire->read();
  uint8_t wordBot = this->_wire->read();

  // this->_wire->beginTransmission(this->_readAddr);
  // this->_wire->write(command);
  // this->_wire->endTransmission();
  // Serial.println("Command sent, getting status");
  // this->_wire->requestFrom(this->_readAddr, (uint8_t)2);
  // uint8_t wordTop = this->_wire->read();
  // uint8_t wordBot = this->_wire->read();
  Serial.print(wordTop);
  Serial.print(' ');
  Serial.println(wordBot);
  tempWord.top = wordTop;
  tempWord.bot = wordBot;
  Serial.print("Status returned - ");
  Serial.println(tempWord.word);
}
#pragma endregion

#pragma region Utils
void SIC451::ParseStatus(uint16_t value)
{
  this->_status.OTHER_Faults = bitRead(value, 0);
  this->_status.COMM_EVENT = bitRead(value, 1);
  this->_status.TEMP_Faults = bitRead(value, 2);
  this->_status.VIN_UV_Fault = bitRead(value, 3);
  this->_status.IOUT_OC_Fault = bitRead(value, 4);
  this->_status.VOUT_OV_Fault = bitRead(value, 5);
  this->_status.Unit_OFF = bitRead(value, 6);
  this->_status.BUSY = bitRead(value, 7);
  this->_status.UNKNOWN_Fault = bitRead(value, 8);
  this->_status.PWR_GOOD = bitRead(value, 11);
  this->_status.MFR_Specific = bitRead(value, 12);
  this->_status.Input_Faults = bitRead(value, 13);
  this->_status.IOUT_Faults = bitRead(value, 14);
  this->_status.VOUT_Faults = bitRead(value, 15);
}
#pragma endregion

#pragma region Testing
uint16_t SIC451::GetVOUT_TEST()
{
  tempWord.word = 0;
  this->ReadCommand(SICConsts::SICRegisters::READ_IOUT);
  return tempWord.word;
}
#pragma endregion