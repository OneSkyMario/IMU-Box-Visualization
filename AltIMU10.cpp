#include "AltIMU10.h"

AltIMU10::AltIMU10(const std::string &bus)
    : accelGyro(bus, 0x6B), mag(bus, 0x1E) {}

void AltIMU10::init() {
    // LSM6DS33:gyroscope + accelerometer
    accelGyro.writeByte(0x10, 0x20); // CTRL1_XL
    accelGyro.writeByte(0x11, 0x24); // CTRL2_G 

    // LIS3MDL: for magnetometer
    mag.writeByte(0x20, 0x6C); 
    mag.writeByte(0x21, 0x00); 
    mag.writeByte(0x22, 0x00); 
}

void AltIMU10::readAccelGyro(float &ax, float &ay, float &az,
                              float &gx, float &gy, float &gz) {
    int16_t gxRaw = accelGyro.read16(0x22, 0x23);
    int16_t gyRaw = accelGyro.read16(0x24, 0x25);
    int16_t gzRaw = accelGyro.read16(0x26, 0x27);
    int16_t axRaw = accelGyro.read16(0x28, 0x29);
    int16_t ayRaw = accelGyro.read16(0x2A, 0x2B);
    int16_t azRaw = accelGyro.read16(0x2C, 0x2D);

    gx = gxRaw * GYRO_SENS;
    gy = gyRaw * GYRO_SENS;
    gz = gzRaw * GYRO_SENS;

    ax = axRaw * ACCEL_SENS;
    ay = ayRaw * ACCEL_SENS;
    az = azRaw * ACCEL_SENS;
}

void AltIMU10::readMag(float &mx, float &my, float &mz) {
    int16_t mxRaw = mag.read16(0x28, 0x29);
    int16_t myRaw = mag.read16(0x2A, 0x2B);
    int16_t mzRaw = mag.read16(0x2C, 0x2D);

    mx = mxRaw * MAG_SENS;
    my = myRaw * MAG_SENS;
    mz = mzRaw * MAG_SENS;
}
