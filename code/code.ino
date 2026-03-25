#include <Wire.h>

#include "motors.h"
#include "IMU.h"
#include "kalman.h"
#include "PD.h"

// 1 == Calibration, 0 == Normal Operation
#define MODE 1
#define LOOPS 1000 
#define STARTUP_TIME 3000

// TODO: Make LED blinking non blocking.
bool calibrated false;

void fatal_error(const char *msg) {
  Serial.println(msg);
  disable_motors();

  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

void setup() {
  // put your setup code here, to run once:

  // Setup LED Pin as debug pin
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize Serial
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin();
  Wire.setClock(400*1000);


  if (setup_motors({9,10,11}, {14,15,16}, 2)) {
    fatal_error("Failed to setup motors.");
  } 

  if (setup_IMU()) {
    fatal_error("Failed to setup IMU.");
  }

  #if MODE==0
  enable_motors();
  #endif
  
  #if MODE==1
  disable_motors();
  #endif

  delay(STARTUP_TIME);
}

void loop() {
  // put your main code here, to run repeatedly:

  static unsigned long last = 0;
  if (micros() - last < 2000) {
    return;
  }

  last += 2000;
  
  #if MODE==1
  static int i = LOOPS;

  if (i < LOOPS) {
    struct IMURawData data;
    read_raw_IMU(&data);

    Serial.printf("%f %f %f %f %f %f \n", data.ax, data.ay, data.az, data.gx, data.gy, data.gz);
    i++;
  }

  if (i == LOOPS) {
    if (Serial.available()) {
      auto val = Serial.readStringUntil('\n');
      if (val == "READY") {
        i = 0;
        return;
      }
    }

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  #endif

  #if MODE==0
  
  // TODO: Calibrate the gyros if not yet calibrated
  if (!calibrated) {

  }

  acc_angle 
  float angle = filter(state.gyro_ang_vel, state.acc_angle);
  float u = calculate(angle, state.gyro_ang_vel);
  move_motors(0, dir_motor(u), speed_motor(u));
  move_motors(1, dir_motor(-u), speed_motor(u));
  #endif
}
