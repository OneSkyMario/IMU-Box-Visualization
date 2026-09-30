#include "I2CDevice.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>

I2CDevice::I2CDevice(const std::string &bus, uint8_t address) : addr(address) {
    fd = open(bus.c_str(), O_RDWR);
    if (fd < 0) {
        throw std::runtime_error("Failed to open I2C bus: " + bus);
    }
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        throw std::runtime_error("Failed to set I2C slave address");
    }
}

I2CDevice::~I2CDevice() {
    if (fd >= 0) close(fd);
}

uint8_t I2CDevice::readByte(uint8_t reg) {
    if (write(fd, &reg, 1) != 1) {
        throw std::runtime_error("I2C write (reg select) failed");
    }
    uint8_t value;
    if (read(fd, &value, 1) != 1) {
        throw std::runtime_error("I2C read failed");
    }
    return value;
}

void I2CDevice::writeByte(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    if (write(fd, buf, 2) != 2) {
        throw std::runtime_error("I2C write failed");
    }
}

int16_t I2CDevice::read16(uint8_t regLow, uint8_t regHigh) {
    uint8_t low = readByte(regLow);
    uint8_t high = readByte(regHigh);
    return static_cast<int16_t>((high << 8) | low);
}
