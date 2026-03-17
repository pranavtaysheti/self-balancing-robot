#ifndef _IMU_H
#define _IMU_H

#include <stdint.h>

struct IMUData {
  float ax;
  float ay;
  float az;

  float gx;
  float gy;
  float gz;
};

int setup_IMU();
void read_IMU(IMUData *data);

#endif