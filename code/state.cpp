#include "state.h"

#include <stdint.h>
#include <math.h>

#include "constants.h"

// You may have to change the axis or sign below based on how your IMU is oriented.
#define ACC_VER az
#define ACC_HOR ax
#define ACC_VER2 ay
#define GYRO gy

// TODO: Calibrate gyro and find bias
#define CALIB_GYRO_BIAS 0

void translate_IMU(const IMUData *imu, StateData *ret) {
  float ax = (float)(imu->ACC_HOR);
  float az = (float)(imu->ACC_VER);
  float ay = (float)(imu->ACC_VER2);

  ret->acc_angle = atan2f(ax, sqrtf((az * az) + (ay * ay))) * 57.29578f;
  ret->gyro_ang_vel = (imu->GYRO - CALIB_GYRO_BIAS)/131.0f;
}

