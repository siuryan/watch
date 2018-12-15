#include "Watch.h"

Watch watch;

void setup() {
  watch.init();
  Serial.begin(9600);
}

void loop() {
  watch.update();
  delay(100);
}
