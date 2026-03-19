#ifndef _IMU_H
#define _IMU_H

#include <stdint.h>

struct IMUData {
  float acc_angle;
  float gyro_ang_vel;
};

struct IMURawData {
  float ax;
  float ay;
  float az;

  float gx;
  float gy;
  float gz;
};

int setup_IMU();
void read_IMU(IMUData *data);
void read_raw_IMU(IMURawData *data);
#endif