#pragma once
#include <Arduino.h>
#include <Wire.h>

#define DS3231_REG_TIMEDATE 0x00
#define DS3231_REG_ALARM_ONE 0x07
#define DS3231_REG_ALARM_TWO 0x0B
#define DS3231_REG_CONTROL 0x0E
#define DS3231_REG_STATUS 0x0F
#define DS3231_REG_AGING 0x10
#define DS3231_REG_TEMP 0x11

class DS3231
{
public:
    bool Initialize(TwoWire &_wire = Wire);

private:
    const uint8_t address = 0x68;
    TwoWire *wire;

    void read(uint8_t _register, uint8_t length, uint8_t *values);
    uint8_t read(uint8_t _register);
    void write(uint8_t _register, const uint8_t &value);
    void write(uint8_t _register, const uint8_t &value, const uint8_t &mask);
};
