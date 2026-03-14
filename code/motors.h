#ifndef MOTORS_H
#define MOTORS_H

enum Direction {
  md_ClockWise,
  md_CounterClockWise,
};

struct m_MotorConfig {
  pin_size_t dir1;
  pin_size_t dir2;
  pin_size_t speed;
};

int setup_motors(struct m_MotorConfig m0, struct m_MotorConfig m1, pin_size_t stby);
int move_motors(int motor, Direction dir, char speed);

#endif