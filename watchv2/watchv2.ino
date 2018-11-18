#include "Watch.h"

Watch watch;

void setup() {
  watch.init();
}

void loop() {
  watch.update();

  delay(100);
}
