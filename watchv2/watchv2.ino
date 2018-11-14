#include "Watch.h"

Watch watch(Serial);

void setup() {
  Serial.begin(9600);

  watch.init();
}

void loop() {
  watch.update_screen(false, false, analogRead(A0));

  delay(100);
}
