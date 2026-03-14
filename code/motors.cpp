#include <Arduino.h>
#include "motors.h"


pin_size_t m_stby;
struct m_MotorConfig m_motorConfig[2];

int setup_motors(struct m_MotorConfig m0, struct m_MotorConfig m1, pin_size_t stby) {
  // Run this function inside main setup function.

  m_stby = stby;
  pinMode(stby, OUTPUT);
  
  m_motorConfig[0] = m0;
  pinMode(m0.dir1, OUTPUT);
  pinMode(m0.dir2, OUTPUT);
  pinMode(m0.speed, OUTPUT);

  m_motorConfig[1] = m1;
  pinMode(m1.dir1, OUTPUT);
  pinMode(m1.dir2, OUTPUT);
  pinMode(m1.speed, OUTPUT);

  return 0;
}

int move_motors(int motor, Direction dir, char speed) {
  // Assumes that setup_motor() function has been run in setup() part.
  // This should go in loop().

  PinStatus dirStatusPin1;
  PinStatus dirStatusPin2;
  
  switch (dir) {
    case md_ClockWise:
      dirStatusPin1 = HIGH;
      dirStatusPin2 = LOW;
      break;

    case md_CounterClockWise:
      dirStatusPin1 = LOW;
      dirStatusPin2 = HIGH;
      break;
  }

  digitalWrite(m_motorConfig[motor].dir1, dirStatusPin1);
  digitalWrite(m_motorConfig[motor].dir2, dirStatusPin2);
  analogWrite(m_motorConfig[motor].speed, speed);

  return 0;
}