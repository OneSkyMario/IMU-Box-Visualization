#ifndef ALTIMU10_H
#define ALTIMU10_H

#include "I2CDevice.h"

class AltIMU10 {
public:
    AltIMU10(const std::string &bus = "/dev/i2c-1");

    void init();
    void readAccelGyro(float &ax, float &ay, float &az,
                        float &gx, float &gy, float &gz);
    void readMag(float &mx, float &my, float &mz);

private:
    I2CDevice accelGyro; // 0x6B (LSM6DS33)
    I2CDevice mag;        //  0x1E (LIS3MDL)

    //scaling
    static constexpr float GYRO_SENS  = 0.0175f;  
    static constexpr float ACCEL_SENS = 0.000061f; 
    static constexpr float MAG_SENS   = 1.0f / 6842.0f;
};

#endif
