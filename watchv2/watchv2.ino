#include "Watch.h"

Watch watch(Serial);

void setup() {
  Serial.begin(9600);

  watch.init();
}

void loop() {
  watch.update();

  delay(100);
}
