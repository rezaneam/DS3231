#include <DS3231.h>

/*!
 *  @brief  Initializing the libary
 *  @param  _wire TwoWire interface - defalt Wire
 *  @return true if any DS3231 sensor found
 */
bool DS3231::Initialize(TwoWire &_wire)
{
    wire = &_wire;

    return (read(DS3231_REG_STATUS) != 0xFF);
}

void DS3231::GetTime(time_t *time)
{
    tm time_tm = tm();
    uint8_t buffer[7] = {0};
    read(DS3231_REG_TIMEDATE, 7, buffer);
    time_tm.tm_sec = bcd2uint(buffer[0]);
    time_tm.tm_min = bcd2uint(buffer[1]);
    time_tm.tm_hour = bcd2uint24Hour(buffer[2]);
    time_tm.tm_wday = buffer[3];
    time_tm.tm_mday = bcd2uint(buffer[4]);
    time_tm.tm_mon = bcd2uint(buffer[5]);
    time_tm.tm_year = bcd2uint(buffer[6]) + 2000;

    *time = mktime(&time_tm);
}

void DS3231::GetTime(tm *time)
{
    uint8_t buffer[7] = {0};
    read(DS3231_REG_TIMEDATE, 7, buffer);
    time->tm_sec = bcd2uint(buffer[0]);
    time->tm_min = bcd2uint(buffer[1]);
    time->tm_hour = bcd2uint24Hour(buffer[2]);
    time->tm_wday = buffer[3];
    time->tm_mday = bcd2uint(buffer[4]);
    time->tm_mon = bcd2uint(buffer[5]);
    time->tm_year = bcd2uint(buffer[6]) + 2000;
}

void DS3231::SetTime(const tm &time)
{
    write(DS3231_REG_TIMEDATE, uint2bcd(time.tm_sec));
    write(DS3231_REG_TIMEDATE + 1, uint2bcd(time.tm_min));
    write(DS3231_REG_TIMEDATE + 2, uint2bcd(time.tm_hour));
    write(DS3231_REG_TIMEDATE + 4, uint2bcd(time.tm_mday));
    write(DS3231_REG_TIMEDATE + 5, uint2bcd(time.tm_mon + 1));    // Month in tm factor starts with 0 (Jan)
    write(DS3231_REG_TIMEDATE + 6, uint2bcd(time.tm_year - 100)); // Year has 1900 as offset
    uint8_t val = read(DS3231_REG_STATUS);
    write(DS3231_REG_STATUS, val & 0x7F); // Indicate that the time is set
}

bool DS3231::HasValidDateTime()
{
    return !(read(DS3231_REG_STATUS) & 0x80);
}

bool DS3231::IsRunning()
{
    return !(read(DS3231_REG_CONTROL) & 0x80);
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

uint8_t DS3231::uint2bcd(uint8_t uint_val)
{
    return uint_val + 6 * (uint_val / 10);
}

uint8_t DS3231::bcd2uint(int8_t bcd_val)
{
    return bcd_val - 6 * (bcd_val >> 4);
}

uint8_t DS3231::bcd2uint24Hour(uint8_t bcdHour)
{
    if (bcdHour & 0x40)
        return (bcdHour & 0x20) ? bcd2uint(bcdHour & 0x1f) + 12 : bcd2uint(bcdHour & 0x1f);

    return bcd2uint(bcdHour);
}