#include "AltIMU10.h"
#include "MadgwickAHRS.h"
#include <iostream>
#include <unistd.h>

int main() {
    AltIMU10 imu;
    imu.init();

    float ax, ay, az, gx, gy, gz, mx, my, mz;

    while (true) {
        imu.readAccelGyro(ax, ay, az, gx, gy, gz);
        imu.readMag(mx, my, mz);

        MadgwickAHRSupdate(gx, gy, gz, ax, ay, az, mx, my, mz);

        std::cout << "q: " << q0 << " " << q1 << " " << q2 << " " << q3 << std::endl;

        usleep(38000); // ~26Hz
    }
    return 0;
}
