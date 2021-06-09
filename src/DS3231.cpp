#include <DS3231.h>

/*!
 *  @brief  Initializing the libary with auto address detect
 *  @param  _wire TwoWire interface - defalt Wire
 *  @return true if any BMA400 sensor found
 */
bool DS3231::Initialize(TwoWire &_wire)
{
    wire = &_wire;

    return (read(DS3231_REG_STATUS) != 0xFF);
}

// Private methods

void DS3231::read(uint8_t _register, uint8_t length, uint8_t *values)
{
    wire->beginTransmission(address);
    wire->write(_register);
    wire->endTransmission();
    wire->requestFrom(address, length);
    for (uint8_t i = 0; i < length; i++)
        values[i] = wire->read();
}

uint8_t DS3231::read(uint8_t _register)
{
    wire->beginTransmission(address);
    wire->write(_register);
    wire->endTransmission();
    wire->requestFrom(address, (uint8_t)1);
    return wire->read();
}

void DS3231::write(uint8_t _register, const uint8_t &value)
{
    wire->beginTransmission(address);
    wire->write((uint8_t)_register);
    wire->write((uint8_t)value);
    wire->endTransmission();
}

void DS3231::write(uint8_t _register, const uint8_t &value, const uint8_t &mask)
{
    uint8_t val = (read(_register) & mask) | value;
    wire->beginTransmission(address);
    wire->write((uint8_t)_register);
    wire->write((uint8_t)val);
    wire->endTransmission();
}