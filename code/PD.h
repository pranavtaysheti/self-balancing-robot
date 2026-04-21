#ifndef _PD_H
#define _PD_H

#include <stdint.h>

#include "direction.h"

float calculate(float angle, float ang_vel);
enum Direction dir_motor(float u);
uint8_t speed_motor(float u);

#endif