#ifndef _IMU_H
#define _IMU_H

#include <stdint.h>

struct IMUData {
  int16_t ax;
  int16_t ay;
  int16_t az;

  int16_t gx;
  int16_t gy;
  int16_t gz;
};

int setup_IMU();
int read_IMU(IMUData *data);

#endif