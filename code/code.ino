#include "motors.h"
#include "IMU.h"
#include "state.h"
#include "kalman.h"
#include "PD.h"

void fatal_error(const char *msg) {
  Serial.println(msg);

  disable_motors();

  pinMode(LED_BUILTIN, OUTPUT);
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  if (setup_motors({9,10,11}, {14,15,16}, 2)) {
    fatal_error("Failed to setup motors.");
  }

  if (setup_IMU()) {
    fatal_error("Failed to setup IMU.");
  }

  enable_motors();
}

void loop() {
  // put your main code here, to run repeatedly:

  struct IMUData data;
  read_IMU(&data);

  struct StateData state;
  translate_IMU(&data, &state);

  float angle = filter(state.gyro_ang_vel, state.acc_angle);
  float u = calculate(angle, state.gyro_ang_vel);
  move_motors(0, dir_motor(u), speed_motor(u));
  move_motors(1, dir_motor(-u), speed_motor(u));
}
