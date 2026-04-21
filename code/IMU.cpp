#include "IMU.h"

#include <Arduino.h>
#include <float.h>
#include <math.h>

#include "MPU6050_light.h"
#include "config.h"

MPU6050 mpu(Wire);

int setup_IMU() {
  mpu.setAccOffsets(ACC_X_BIAS, ACC_Y_BIAS, ACC_Z_BIAS);
  byte status = mpu.begin();
  return status;
}

void read_IMU(IMUData *data) {
  mpu.update();

  data->acc_angle = mpu.getAccAngleY();
  data->gyro_ang_vel = mpu.getGyroY();
}

void read_raw_IMU(IMURawData *data) {
  mpu.fetchData();

  data->ax = mpu.getAccX();
  data->ay = mpu.getAccY();
  data->az = mpu.getAccZ();

  data->gx = mpu.getGyroX();
  data->gy = mpu.getGyroY();
  data->gz = mpu.getGyroZ();
}

// Return Codes:
// false: Calibrating...
// true: Calibrated
bool calibrate_gyro() {
  static IMURawData buffer[100];
  static int curr = 0;

  bool result = false;
  int i = curr % 100;

  read_raw_IMU(&buffer[i]);

  float x_high = -FLT_MAX, x_low = +FLT_MAX;
  float y_high = -FLT_MAX, y_low = +FLT_MAX;
  float z_high = -FLT_MAX, z_low = +FLT_MAX;

  x_high = fmaxf(x_high, buffer[i].gx);
  x_low = fminf(x_low, buffer[i].gx);

  y_high = fmaxf(y_high, buffer[i].gy);
  y_low = fminf(y_low, buffer[i].gy);

  z_high = fmaxf(z_high, buffer[i].gz);
  z_low = fminf(z_low, buffer[i].gz);

  if (curr >= 100) {
    if (x_high - x_low < GYRO_DELTA && y_high - y_low < GYRO_DELTA &&
        z_high - z_low < GYRO_DELTA) {
      float x = (x_high + x_low) / 2;
      float y = (y_high + y_low) / 2;
      float z = (z_high + z_low) / 2;

      mpu.setGyroOffsets(x, y, z);
      result = true;
    }
  }

  curr++;
  return result;
}