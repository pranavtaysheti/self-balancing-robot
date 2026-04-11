#include <Wire.h>
#include <float.h>

#include "IMU.h"
#include "LED.h"
#include "PD.h"
#include "config.h"
#include "kalman.h"
#include "motors.h"
#include "state.h"

// 1 == Calibration, 0 == Normal Operation
#define MODE 0

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

  // Set the state
  globalState = S_WAITING_CALIBRATION;
  // Setup LED Pin as debug pin
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize Serial
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin();
  Wire.setClock(400 * 1000);

  // Initialize IMU at default SPI ports
  if (setup_IMU()) {
    fatal_error("Failed to setup IMU.");
  }

  // Initialize Motors
  if (setup_motors({M0_D1, M0_D2, M0_SPD}, {M1_D1, M1_D2, M1_SPD}, M_STBY)) {
    fatal_error("Failed to setup motors.");
  }

#if MODE == 0
  enable_motors();
#endif

#if MODE == 1
  disable_motors();
#endif

  // Wait for IMU to stabilize
  delay(STARTUP_TIME);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Update the LED State
  update_LED();

  // Wait for 2ms to pass from last run.
  static unsigned long last = 0;
  if (micros() - last < DT * 1000) {
    return;
  }

  last += 2000;

// Calibration Mode
#if MODE == 1
  static int i = LOOPS;

  if (i < LOOPS) {
    struct IMURawData data;
    read_raw_IMU(&data);

    Serial.printf("%f %f %f %f %f %f \n", data.ax, data.ay, data.az, data.gx,
                  data.gy, data.gz);

    i++;
    return
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
#endif

// Normal Operation Mode
#if MODE == 0

  // Calibration Routine: Calibrate the gyros if not yet calibrated
  if (globalState == S_WAITING_CALIBRATION) {
    globalState = S_CALIBRATING;

    // // TODO: Gyro Calibrating Code
    // static IMUData buffer[100];
    // static curr = 0;
    // read_IMU(buffer[curr % 100]);
    // curr++;
    //
    // if (curr >= 100) {
    //   float x_high = FLT_MAX, x_low = -FLT_MAX;
    //   float y_high = FLT_MAX, y_low = -FLT_MAX;
    //   float z_high = FLT_MAX, z_low = -FLT_MAX;
    //
    //   for (int i = 0; i < 100; i++) {
    //
    //   }
    // }
    // return;
  }

  globalState = S_CALIBRATED;

  // Robot Balancing Routine
  static struct IMUData robotState;
  read_IMU(&robotState);

  float angle = filter(robotState.gyro_ang_vel, robotState.acc_angle);
  float u = calculate(angle, robotState.gyro_ang_vel);

  move_motors(M_LEFT, dir_motor(u), speed_motor(u));
  move_motors(M_RIGHT, dir_motor(-u), speed_motor(u));
#endif
}
