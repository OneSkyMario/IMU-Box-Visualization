[README_IMU_Box_Visualization.md](https://github.com/user-attachments/files/32864643/README_IMU_Box_Visualization.md)
# IMU Box Visualization

A C++ project for reading a 9-DOF IMU from a Raspberry Pi, estimating orientation with the Madgwick AHRS algorithm, and visualizing the resulting quaternion as a real-time 3D cube using OpenGL/FreeGLUT.

This project was developed for **ROBT 402 - Robot/Mechatronic System Design, Project Assignment 2**.

## Overview

The software is organized into small C++ modules with separate `.h` and `.cpp` files:

- `I2CDevice` - low-level Linux I2C access.
- `AltIMU10` - IMU hardware abstraction, sensor configuration, raw register reads, and conversion to physical units.
- `MadgwickAHRS` - 9-DOF sensor fusion and quaternion estimation.
- `main.cpp` - terminal application that continuously prints quaternion values.
- `imu_cube.cpp` - real-time OpenGL visualization driven by the IMU quaternion.
- `cube_quat.cpp` - standalone quaternion cube viewer for testing the visualization without the IMU.

## Hardware

The project uses a Raspberry Pi with an AltIMU-style sensor board containing:

| Sensor | Function | I2C address |
|---|---|---:|
| LSM6DS33 | 3-axis accelerometer + gyroscope | `0x6B` |
| LIS3MDL | 3-axis magnetometer | `0x1E` |

The code communicates through Linux I2C bus 1 (`/dev/i2c-1`).

## Sensor configuration

The program configures the sensors for the settings used in the assignment:

- Accelerometer: **26 Hz**, **+/-2 g**
- Gyroscope: **26 Hz**, **+/-500 deg/s**
- Magnetometer: **5 Hz**, **+/-4 gauss**, continuous conversion

Raw 16-bit measurements are converted using the sensor sensitivities used by the project:

- Accelerometer: `0.000061 g/LSB`
- Gyroscope: `0.0175 deg/s/LSB`
- Magnetometer: `1 / 6842 gauss/LSB`

The Madgwick filter runs at approximately **26 Hz** and produces orientation as a quaternion `(w, x, y, z)`.

## Requirements

On Raspberry Pi OS / Debian-based Linux:

```bash
sudo apt update
sudo apt install -y g++ i2c-tools freeglut3-dev mesa-utils
```

Make sure I2C is enabled:

```bash
sudo raspi-config
```

Then verify the IMU is visible:

```bash
i2cdetect -y 1
```

You should see the LSM6DS33 at `0x6B` and the LIS3MDL at `0x1E`.

## Build

### Quaternion terminal output

```bash
g++ -std=c++17 -O2 \
    main.cpp I2CDevice.cpp AltIMU10.cpp MadgwickAHRS.cpp \
    -o imu_test
```

Run:

```bash
./imu_test
```

Example output:

```text
q: 0.999996 0.002934 -0.000027 0.000406
q: 0.999802 0.019683 -0.000062 0.002756
```

### IMU-driven 3D cube

```bash
g++ -std=c++17 -O2 \
    imu_cube.cpp I2CDevice.cpp AltIMU10.cpp MadgwickAHRS.cpp \
    -lglut -lGL -lGLU \
    -o imu_cube
```

Run:

```bash
./imu_cube
```

The OpenGL window displays a cube whose orientation follows the estimated IMU quaternion in real time.

### Standalone quaternion viewer

`cube_quat.cpp` can be used to test the visualization without sensor hardware:

```bash
g++ -std=c++17 -O2 cube_quat.cpp -lglut -lGL -lGLU -o cube_quat
./cube_quat
```

You can also provide an initial quaternion:

```bash
./cube_quat 0.9239 0 0.3827 0
```

## Data flow

```text
LSM6DS33 + LIS3MDL
        |
        v
    I2CDevice
        |
        v
     AltIMU10
        |
        v
Scaled accel / gyro / magnetometer data
        |
        v
   Madgwick AHRS
        |
        v
Quaternion (w, x, y, z)
        |
        +--> Terminal output (`imu_test`)
        |
        +--> OpenGL cube (`imu_cube`)
```

## Notes

- The current implementation expects the IMU on `/dev/i2c-1`.
- The LSM6DS33 and LIS3MDL register configuration is performed automatically by `AltIMU10::init()`.
- Each sensor axis is read as a signed 16-bit value assembled from low and high 8-bit registers.
- `MadgwickAHRSupdate()` uses accelerometer, gyroscope, and magnetometer measurements for 9-DOF orientation estimation.

## Repository

https://github.com/OneSkyMario/IMU-Box-Visualization
