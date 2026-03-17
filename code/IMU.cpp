#include "IMU.h"

#include <Arduino.h>
#include <Wire.h>

#include "MPU6050_light.h"

MPU6050 mpu(Wire);

int setup_IMU() {
  Wire.begin();
  Wire.setClock(100*1000);
  byte status = mpu.begin();
  for (int i = 0; i < 10; i++) {
    Serial.println(status);
    delay(500);
  }

  return status;
}

void read_IMU(IMUData *data) {
  mpu.update();
  
  data->ax = mpu.getAccX();
  data->ay = mpu.getAccY();
  data->az = mpu.getAccZ();

  data->gx = mpu.getGyroX();
  data->gy = mpu.getGyroY();
  data->gz = mpu.getGyroZ();
}