#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <cstdint>
#include <string>

class I2CDevice {
public:
    I2CDevice(const std::string &bus, uint8_t address);
    ~I2CDevice();

    uint8_t readByte(uint8_t reg);
    void writeByte(uint8_t reg, uint8_t value);
    int16_t read16(uint8_t regLow, uint8_t regHigh);

private:
    int fd;
    uint8_t addr;
};

#endif
