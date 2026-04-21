#ifndef _CONFIG_H
#define _CONFIG_H

// time interval between each loop in ms.
#define DT 2

// Calibration Values
// XX_SCALE values are not used now. but maybe used in future someday when I
// don't feel lazy.
#define ACC_X_BIAS 0.0f
#define ACC_X_SCALE 1.0f

#define ACC_Y_BIAS 0.0f
#define ACC_Y_SCALE 1.0f

#define ACC_Z_BIAS 0.0895f
#define ACC_Z_SCALE 1.0f

// No. of times data is taken to calibrate accelerometer during calibration
// routine
#define LOOPS 1000

// No. of ms to wait for IMU to stabilize
#define STARTUP_TIME 3000

// Common Pins: Motors
#define M0_D1 9
#define M0_D2 10
#define M0_SPD 11

#define M1_D1 14
#define M1_D2 15
#define M1_SPD 16

#define M_STBY 2

// Gyro Calibration Acceptable Delta
#define GYRO_DELTA 0.1

#endif