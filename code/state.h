#ifndef _STATE_H
#define _STATE_H

enum State {
  S_WAITING_CALIBRATION,
  S_CALIBRATING,
  S_CALIBRATED,
};

extern enum State globalState;
#endif