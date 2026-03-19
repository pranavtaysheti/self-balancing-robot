#include "IMU.h"

#include <Arduino.h>

#include "common.h"
#include "MPU6050_light.h"

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