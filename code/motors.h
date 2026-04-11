#ifndef _MOTORS_H
#define _MOTORS_H

#include <Arduino.h>
#include <stdint.h>

#include "common.h"

enum Motor {
  M_LEFT,  
  M_RIGHT,
};

struct m_MotorConfig {
  pin_size_t dir1;
  pin_size_t dir2;
  pin_size_t speed;
};

int setup_motors(struct m_MotorConfig m0, struct m_MotorConfig m1, pin_size_t stby);
void enable_motors();
void disable_motors();
int move_motors(enum Motor motor, enum Direction dir, uint8_t speed);

#endif