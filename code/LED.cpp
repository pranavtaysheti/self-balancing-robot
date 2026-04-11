#include "LED.h"

#include <Arduino.h>
#include "state.h"

// LED CODES:
// 100ms blinking - fatal error.
// 500ms blinking - need to calibrate.
// 250ms blinking - calibrating.
// contiuous      - normal operation / calibration complete.
void update_LED() {
  static unsigned long last = 0;

  switch(globalState) {
    case S_WAITING_CALIBRATION:
      if (millis() - last > 500) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        last = millis();
      }
      break;
    
    case S_CALIBRATING:
      if (millis() - last > 250) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        last = millis();
      }
      break;
    
    case S_CALIBRATED:
      digitalWrite(LED_BUILTIN, HIGH);
  }
}