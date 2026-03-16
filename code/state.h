#ifndef _STATE_H
#define _STATE_H

#include "IMU.h"

struct StateData {
  float acc_angle;
  float gyro_ang_vel;
};

void translate_IMU(const IMUData *imu, StateData *ret);

#endif