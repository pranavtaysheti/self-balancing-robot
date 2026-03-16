// This is calibration routine.

#include "motors.h"
#include "IMU.h"

// No. of loops after which calibration stops.
#define LOOPS 1000

void fatal_error(const char *msg) {
  Serial.println(msg);

  disable_motors();

  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);

  if (setup_motors({9,10,11}, {14,15,16}, 2)) {
    fatal_error("Failed to setup motors.");
  }

  if (setup_IMU()) {
    fatal_error("Failed to setup IMU.");
  }

  disable_motors();
  delay(2000);
}

// This functions sends gyro reading using Serial. User is expected to process 
// the data on PC using whatever algorithm he prefers.
void loop() {
  // put your main code here, to run repeatedly:
  static int i = 0;

  if (i == LOOPS) {
    Serial.println("CALIBRATION ROUTINE ENDED SUCCESSFULLY!");
  
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    return;
  }

  IMUData data;
  read_IMU(&data);
  
  Serial.printf("GYRO READINGS: %d %d %d\n", (int)(data.gx), (int)(data.gy), (int)(data.gz));
  delay(1);
  i++;
}
