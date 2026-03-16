#include "IMU.h"

#include <Arduino.h>
#include <Wire.h>

#include "MPU6050.h"

MPU6050 imu;

int setup_IMU() {
  Wire.begin();
  
  imu.initialize();
  return !imu.testConnection();
}

void read_IMU(IMUData *data) {
  imu.getMotion6(&data->ax, &data->ay, &data->az, &data->gx, &data->gy, &data->gz);
}

void translate();