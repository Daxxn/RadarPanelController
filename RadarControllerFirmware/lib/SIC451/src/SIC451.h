#pragma once

#include <Arduino.h>
#include <Wire.h>

#define PMBUS_BUFFER_LEN 16

enum PMBUS_COMMAND
{
    OPERATION = 0x01,
    ON_OFF_CONFIG = 0x02,
    CLEAR_FAULTS = 0x03,
    WRITE_PROTECT = 0x10,
    STORE_USER_ALL = 0x15,
    RESTORE_USER_ALL = 0x16,
    CAPABILITY = 0x19,
    SMBALERT_MASK = 0x1B,
    VOUT_MODE = 0x20,
    VOUT_COMMAND = 0x21,
    VOUT_TRIM = 0x22,
    VOUT_MAX = 0x24,
    VOUT_MARGIN_HIGH = 0x25,
    VOUT_MARGIN_LOW = 0x26,
    VOUT_TRANSITION_RATE = 0x27,
    VOUT_SCALE_LOOP = 0x29,
    FREQUENCY_SWITCH = 0x33,
    VIN_ON = 0x35,
    VIN_OFF = 0x36,
    INTERLEAVE = 0x37,
    VOUT_OV_FAULT_LIMIT = 0x40,
    VOUT_OV_FAULT_RESPONSE = 0x41,
    VOUT_OV_WARN_LIMIT = 0x42,
    VOUT_UV_WARN_LIMIT = 0x43,
    VOUT_UV_FAULT_LIMIT = 0x44,
    VOUT_UV_FAULT_RESPONSE = 0x45,
    IOUT_OC_FAULT_LIMIT = 0x46,
    IOUT_OC_FAULT_RESPONSE = 0x47,
    IOUT_OC_WARN_LIMIT = 0x4A,
    OT_FAULT_RESPONSE = 0x50,
    OT_WARN_LIMIT = 0x51,
    VIN_OV_FAULT_LIMIT = 0x55,
    VIN_OV_FAULT_RESPONSE = 0x56,
    VIN_UV_WARN_LIMIT = 0x58,
    IIN_OC_WARN_LIMIT = 0x5D,
    POWER_GOOD_ON = 0x5E,
    POWER_GOOD_OFF = 0x5F,
    TON_DELAY = 0x60,
    TON_RISE = 0x61,
    TON_MAX_FAULT_LIMIT = 0x62,
    TON_MAX_FAULT_RESPONSE = 0x63,
    TOFF_DELAY = 0x64,
    TOFF_FALL = 0x65,
    TOFF_MAX_WARN_LIMIT = 0x66,
    STATUS_BYTE = 0x78,
    STATUS_WORD = 0x79,
    STATUS_VOUT = 0x7A,
    STATUS_IOUT = 0x7B,
    STATUS_INPUT = 0x7C,
    STATUS_TEMPERATURE = 0x7D,
    STATUS_CML = 0x7E,
    STATUS_MFR_SPECIFIC = 0x80,
    READ_VIN = 0x88,
    READ_IIN = 0x89,
    READ_VOUT = 0x8B,
    READ_IOUT = 0x8C,
    READ_TEMPERATURE = 0x8D,
    READ_DUTY_CYCLE = 0x94,
    READ_POUT = 0x96,
    PMBUS_REV = 0x98,
    MFR_ID = 0x99,
    MFR_MODEL = 0x9A,
    MFR_REV = 0x9B,
    MFR_SERIAL = 0x9E,
    IC_DEVICE_ID = 0xAD,
    IC_DEVICE_REV = 0xAE,
};

enum PMBUS_WRITEPROTECT
{
  ENABLE = 0b0,
  ALL_DISABLE = 0b10000000,
  OPERATION_ENABLE = 0b01000000,
  VOUT_OP_ON_CONFIG_EN = 0b00100000
};

namespace SICConsts
{
  const uint8_t DEFAULT_EXP = 0b10111;
  namespace ValueRanges
  {
    const uint16_t MAX_LINEAR11_EXP = 0x7FF;
    const int MAX_LINEAR11_NUM = 0b11111;
  } // namespace ValueRanges
} // namespace SICConsts

// union WordUnion
// {
//   uint8_t top;
//   uint8_t bot;
//   uint16_t word;
// };


// union WordUnion
// {
//   uint8_t buffer[2] = {0, 0};
//   uint16_t value;
// };


struct LinearVoltage
{
  LinearVoltage() {};
  uint8_t exp = SICConsts::DEFAULT_EXP;
  uint16_t base = 0;
  double value;
  double Calc()
  {
    value = base * pow(2, -exp);
    return value;
  }
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
  LinearVoltage VOUT = LinearVoltage();
  double IOUT = 0;
  double IN = 0;
  double TEMP = 0;
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
  ~SIC451();

  bool GetPGood();
  bool GetAlert();

  // Only use if the interrupt is not configured.
  bool CheckAlert();

  void EnablePower();
  void EnablePower(bool en);

  void GetStatus();
  SIC451Status CurrentStatus() {return this->_status;};

  uint16_t TEST();

  double GetVOUT();

  void SetWriteProtect(PMBUS_WRITEPROTECT protect);

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
  SIC451Monitors _monitors;

  // WordUnion _word;
  uint8_t *_buffer;

  void WriteCommand(PMBUS_COMMAND command, uint8_t data);
  void ReadCommand(PMBUS_COMMAND command, uint8_t *buffer, uint8_t len);
  void ReadCommand(PMBUS_COMMAND command, size_t len);
  uint8_t ReadByteCommand(PMBUS_COMMAND command);

  void ParseStatus(uint16_t value);
};