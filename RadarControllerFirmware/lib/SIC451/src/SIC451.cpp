#include <Arduino.h>
#include <Wire.h>
#include "SIC451.h"
#include "Utils.h"

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

  this->_buffer = new uint8_t[PMBUS_BUFFER_LEN];
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

SIC451::~SIC451()
{
  delete[] this->_buffer;
}

#pragma region PMBus Functions
void SIC451::GetStatus()
{
  this->ReadCommand(PMBUS_COMMAND::STATUS_WORD, 2);
  // ParseStatus(this->_word.value);
  ParseStatus(CombineBytes(this->_buffer));
  Serial.print(this->_status.BUSY);
  Serial.print(' ');
  Serial.print(this->_status.PWR_GOOD);
  Serial.print(' ');
  Serial.print(this->_status.Input_Faults);
  Serial.print(' ');
  Serial.println(this->_status.OTHER_Faults);
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

void SIC451::SetWriteProtect(PMBUS_WRITEPROTECT protect)
{
  this->WriteCommand(PMBUS_COMMAND::WRITE_PROTECT, protect);
}

double SIC451::GetVOUT()
{
  // Not working...
  // uint8_t modeByte = this->ReadByteCommand(PMBUS_COMMAND::VOUT_MODE);
  // uint8_t exp = modeByte & 0b11111;
  // delay(50);
  this->ReadCommand(PMBUS_COMMAND::READ_VOUT, 2);
  // uint16_t voutBytes = this->_word.value;
  uint16_t voutBytes = CombineBytes(this->_buffer);
  this->_monitors.VOUT.base = voutBytes;
  Serial.print(this->_monitors.VOUT.exp);
  Serial.print(' ');
  Serial.println(voutBytes);
  return this->_monitors.VOUT.Calc();
}
#pragma endregion

#pragma region Internal PMBus Comms
void SIC451::WriteCommand(PMBUS_COMMAND command, uint8_t data)
{
  this->_wire->beginTransmission(this->_address);
  this->_wire->write(command);
  this->_wire->write(data);
  this->_wire->endTransmission();
}

void SIC451::ReadCommand(PMBUS_COMMAND command, uint8_t* buffer, uint8_t len)
{
  this->_wire->beginTransmission(this->_address);
  this->_wire->write(command);
  this->_wire->endTransmission();
  this->_wire->requestFrom(this->_address, len);
  this->_wire->readBytes(buffer, len);
}

void SIC451::ReadCommand(PMBUS_COMMAND command, size_t len)
{
  // this->_wire->requestFrom(this->_address, len);
  // this->_wire->write(command);
  // this->_wire->readBytes(this->_buffer, len);
  this->_wire->beginTransmission(this->_address);
  this->_wire->write(command);
  this->_wire->endTransmission();
  this->_wire->requestFrom(this->_address, len);
  this->_wire->readBytes(this->_buffer, len);
}

uint8_t SIC451::ReadByteCommand(PMBUS_COMMAND command)
{
  // this->_wire->requestFrom(this->_address, (uint8_t)1);
  // this->_wire->write(command);
  // return this->_wire->read();
  this->_wire->beginTransmission(this->_address);
  this->_wire->write(command);
  this->_wire->endTransmission();
  this->_wire->requestFrom(this->_readAddr, (uint8_t)1);
  return this->_wire->read();
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
uint16_t SIC451::TEST()
{
  this->WriteCommand(PMBUS_COMMAND::WRITE_PROTECT, PMBUS_WRITEPROTECT::ENABLE);
  Serial.println(this->ReadByteCommand(PMBUS_COMMAND::WRITE_PROTECT));
  return 0;
}
#pragma endregion