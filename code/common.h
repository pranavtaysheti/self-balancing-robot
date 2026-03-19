#ifndef _CONSTANTS_H
#define _CONSTANTS_H

// time interval between each loop in ms.
#define DT 2

// Calibration Values
// XX_SCALE values are not used now. but maybe used in future someday when I don't feel lazy.
#define ACC_X_BIAS 0.0f
#define ACC_X_SCALE 1.0f

#define ACC_Y_BIAS 0.0f
#define ACC_Y_SCALE 1.0f

#define ACC_Z_BIAS 0.0895f
#define ACC_Z_SCALE 1.0f

enum Direction {
  md_ClockWise,
  md_CounterClockWise,
};

#endif