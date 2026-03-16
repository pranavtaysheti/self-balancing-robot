// Credits: 
// This code is based on famous TKJ Electronics' Kalman Library:
// https://github.com/TKJElectronics/KalmanFilter

#include "kalman.h"

#include "constants.h"

const float DT_S = DT/1000.0f;

//TODO: Tune Values.
const float Q_ANGLE = 0.001;
const float Q_BIAS = 0.003;
const float R_ANGLE = 0.03;

float filter(float gyro_ang_vel, float acc_angle) {
  // Initialize State
  static float angle = 0;
  static float bias = 0;
  static float P[2][2] = {{1,0}, {0,1}};

  // Step 0: Sanitize data from gyro
  float angVel = gyro_ang_vel - bias;

  // Step 1: Project the state ahead
  float prj_angle = angle + gyro_ang_vel*DT_S;
  float prj_bias = bias; //bias is assumed to be static.

  // Step 2: Update state covariance:
  // Some time has passed since last estimation so variance should increase.
  P[0][0] += DT_S*DT_S*P[1][1] - DT_S*P[1][0] - DT_S*P[0][1] + DT_S*Q_ANGLE;
  P[0][1] -= P[1][1] * DT_S;
  P[1][0] -= P[1][1] * DT_S;
  P[1][1] += DT_S*Q_BIAS;

  // Step 3: calculate innovation (Delta between our prediction and measurement from accelerometer)
  float y_angle = acc_angle - prj_angle;

  // Step 4: Calculate Kalman Gain
  float S = P[0][0] + R_ANGLE;

  // Step 5:
  float K[2];
  K[0] = P[0][0]/S;
  K[1] = P[1][0]/S;

  // Step 6:
  angle = prj_angle + K[0] * y_angle;
  bias = prj_bias + K[1] * y_angle;

  // Step 7:
  float P00 = P[0][0];
  float P01 = P[0][1];

  P[0][0] -= K[0] * P00;
  P[0][1] -= K[0] * P01;
  P[1][0] -= K[1] * P00;
  P[1][1] -= K[1] * P01;

  return angle;
}