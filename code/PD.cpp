#include "PD.h"

#include <stdlib.h>

// TODO: Calibrate values for KP and KD. current values are placeholders.
#define KP 10.0f
#define KD 1.0f
#define GAIN 1
#define THRESHOLD 40

float calculate(float angle, float ang_vel) {
  return KP*angle + KD*ang_vel;
}

enum Direction dir_motor(float u) {
  if (u > 0) {
    return D_CLOCKWISE;
  }

  return D_COUNTER_CLOCKWISE;
}

uint8_t speed_motor(float u) {
  int v = abs((int)u);
  if (v < THRESHOLD) {
    v = THRESHOLD;
  } else if (v > 255) {
    v = 255;
  }

  return v;
}