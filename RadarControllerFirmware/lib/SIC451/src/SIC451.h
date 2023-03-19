#pragma once

#include <Arduino.h>
#include <Wire.h>

namespace SICConsts
{
  namespace SICRegisters
  {
    const uint8_t OPERATION = 0x01;
    const uint8_t ON_OFF_CONFIG = 0x02;
    const uint8_t CLEAR_FAULTS = 0x03;
    const uint8_t WRITE_PROTECT = 0x10;
    const uint8_t STORE_USER_ALL = 0x15;
    const uint8_t RESTORE_USER_ALL = 0x16;
    const uint8_t CAPABILITY = 0x19;
    const uint8_t SMBALERT_MASK = 0x1B;
    const uint8_t VOUT_MODE = 0x20;
    const uint8_t VOUT_COMMAND = 0x21;
    const uint8_t VOUT_TRIM = 0x22;
    const uint8_t VOUT_MAX = 0x24;
    const uint8_t VOUT_MARGIN_HIGH = 0x25;
    const uint8_t VOUT_MARGIN_LOW = 0x26;
    const uint8_t VOUT_TRANSITION_RATE = 0x27;
    const uint8_t VOUT_SCALE_LOOP = 0x29;
    const uint8_t FREQUENCY_SWITCH = 0x33;
    const uint8_t VIN_ON = 0x35;
    const uint8_t VIN_OFF = 0x36;
    const uint8_t INTERLEAVE = 0x37;
    const uint8_t VOUT_OV_FAULT_LIMIT = 0x40;
    const uint8_t VOUT_OV_FAULT_RESPONSE = 0x41;
    const uint8_t VOUT_OV_WARN_LIMIT = 0x42;
    const uint8_t VOUT_UV_WARN_LIMIT = 0x43;
    const uint8_t VOUT_UV_FAULT_LIMIT = 0x44;
    const uint8_t VOUT_UV_FAULT_RESPONSE = 0x45;
    const uint8_t IOUT_OC_FAULT_LIMIT = 0x46;
    const uint8_t IOUT_OC_FAULT_RESPONSE = 0x47;
    const uint8_t IOUT_OC_WARN_LIMIT = 0x4A;
    const uint8_t OT_FAULT_RESPONSE = 0x50;
    const uint8_t OT_WARN_LIMIT = 0x51;
    const uint8_t VIN_OV_FAULT_LIMIT = 0x55;
    const uint8_t VIN_OV_FAULT_RESPONSE = 0x56;
    const uint8_t VIN_UV_WARN_LIMIT = 0x58;
    const uint8_t IIN_OC_WARN_LIMIT = 0x5D;
    const uint8_t POWER_GOOD_ON = 0x5E;
    const uint8_t POWER_GOOD_OFF = 0x5F;
    const uint8_t TON_DELAY = 0x60;
    const uint8_t TON_RISE = 0x61;
    const uint8_t TON_MAX_FAULT_LIMIT = 0x62;
    const uint8_t TON_MAX_FAULT_RESPONSE = 0x63;
    const uint8_t TOFF_DELAY = 0x64;
    const uint8_t TOFF_FALL = 0x65;
    const uint8_t TOFF_MAX_WARN_LIMIT = 0x66;
    const uint8_t STATUS_BYTE = 0x78;
    const uint8_t STATUS_WORD = 0x79;
    const uint8_t STATUS_VOUT = 0x7A;
    const uint8_t STATUS_IOUT = 0x7B;
    const uint8_t STATUS_INPUT = 0x7C;
    const uint8_t STATUS_TEMPERATURE = 0x7D;
    const uint8_t STATUS_CML = 0x7E;
    const uint8_t STATUS_MFR_SPECIFIC = 0x80;
    const uint8_t READ_VIN = 0x88;
    const uint8_t READ_IIN = 0x89;
    const uint8_t READ_VOUT = 0x8B;
    const uint8_t READ_IOUT = 0x8C;
    const uint8_t READ_TEMPERATURE = 0x8D;
    const uint8_t READ_DUTY_CYCLE = 0x94;
    const uint8_t READ_POUT = 0x96;
    const uint8_t PMBUS_REV = 0x98;
    const uint8_t MFR_ID = 0x99;
    const uint8_t MFR_MODEL = 0x9A;
    const uint8_t MFR_REV = 0x9B;
    const uint8_t MFR_SERIAL = 0x9E;
    const uint8_t IC_DEVICE_ID = 0xAD;
    const uint8_t IC_DEVICE_REV = 0xAE;
  } // namespace SICRegisters
  namespace ValueRanges
  {
    const uint16_t MAX_LINEAR11_EXP = 0x7FF;
    const int MAX_LINEAR11_NUM = 0b11111;
  } // namespace ValueRanges
} // namespace SICConsts

union WordUnion
{
  uint8_t top;
  uint8_t bot;
  uint16_t word;
};

struct SIC451Status
{
  SIC451Status(){};
  bool VOUT_Faults;
  bool IOUT_Faults;
  bool Input_Faults;
  bool MFR_Specific;
  bool PWR_GOOD;
  bool UNKNOWN_Fault;
  bool BUSY;
  bool Unit_OFF;
  bool VOUT_OV_Fault;
  bool IOUT_OC_Fault;
  bool VIN_UV_Fault;
  bool TEMP_Faults;
  bool COMM_EVENT;
  bool OTHER_Faults;
};

// struct Linear11
// {
//   Linear11(){};
//   Linear11(uint8_t top, uint8_t bot)
//   {
//     this->word.top = top;
//     this->word.bot = bot;
//   };
//   Linear11(uint16_t word)
//   {
//     this->word.word = word;
//   }
//   WordUnion word;
//   double GetValue()
//   {
//     double output = 0;
//     uint8_t num = (this->word.top >> 3) & 0b11111;
//     uint16_t exponent = this->word.word & SICConsts::ValueRanges::MAX_LINEAR11_EXP;
//     output = exponent * pow(2, num);
//     return output;
//   };
// };

// union IEEEFloatUnion
// {
//   uint8_t top;
//   uint8_t bot;
//   float value;
// };

// struct IEEEFloat
// {
//   IEEEFloatUnion fl;
//   IEEEFloat(){};
//   IEEEFloat(uint16_t val)
//   {
//   }
// };

// struct SIC451State
// {
//   float VOUT_Margin_HIGH = 0;
//   float VOUT_Margin_LOW = 0;
//   Linear11 SWTCH_FREQ = 0;
//   Linear11 OT_Fault = 0;
//   Linear11 OT_Warn = 0;
// };

struct SIC451Monitors
{
  float VOUT = 0;
  float IOUT = 0;
  float IN = 0;
  float TEMP = 0;
};

class SIC451
{
public:
  SIC451(
    TwoWire* wire,
    uint8_t address,
    void (*interrupt)(void),
    int pGoodPin,
    int sAlertPin,
    int enablePin
  );
  SIC451(
    TwoWire* wire,
    uint8_t address,
    int pGoodPin,
    int sAlertPin,
    int enablePin
  );

  bool GetPGood();
  bool GetAlert();

  // Only use if the interrupt is not configured.
  bool CheckAlert();

  void EnablePower();
  void EnablePower(bool en);

  void GetStatus();
  SIC451Status CurrentStatus() {return this->_status;};

  uint16_t GetVOUT_TEST();

private:
  TwoWire* _wire;
  uint8_t _address;
  int _pGoodPin;
  int _sAlertPin;
  int _enablePin;

  bool _pGood;
  bool _alert;
  bool _en;

  bool _ignoreENPin;

  uint8_t _writeAddr;
  uint8_t _readAddr;

  void (*_interrupt)(void);

  SIC451Status _status;

  WordUnion tempWord;

  void WriteCommand(uint8_t command, uint8_t data);
  void ReadCommand(uint8_t command, uint8_t *buffer, uint8_t len);
  void ReadCommand(uint8_t command);

  void ParseStatus(uint16_t value);
};