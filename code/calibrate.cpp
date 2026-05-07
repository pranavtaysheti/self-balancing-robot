#include "calibrate.h"
#include "IMU.h"
#include "config.h"

#include <Arduino.h>

static int i = LOOPS;

void calibration_loop() {
  if (i < LOOPS) {
    struct IMURawData data;
    read_raw_IMU(&data);

    Serial.printf("%f %f %f %f %f %f \n", data.ax, data.ay, data.az, data.gx,
                  data.gy, data.gz);

    i++;
    return;
  }

  if (i == LOOPS) {
    if (Serial.available()) {
      auto val = Serial.readStringUntil('\n');
      if (val == "READY") {
        i = 0;
        return;
      }
    }
  }
}