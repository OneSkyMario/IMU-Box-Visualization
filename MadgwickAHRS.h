#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

void MadgwickAHRSupdate(float gx, float gy, float gz,
                         float ax, float ay, float az,
                         float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz,
                            float ax, float ay, float az);

extern volatile float beta;
extern volatile float q0, q1, q2, q3;

#endif
