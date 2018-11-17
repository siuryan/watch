#include "Watch.h"

const byte L_BUTTON_PIN = 7;
const byte R_BUTTON_PIN = 2;

Watch watch(Serial);

bool l_button_debounce = false;
bool r_button_debounce = false;

void setup() {
  Serial.begin(9600);

  pinMode(L_BUTTON_PIN, INPUT_PULLUP);
  pinMode(R_BUTTON_PIN, INPUT_PULLUP);

  watch.init();
}

void loop() {
  bool l_button_pressed = false;
  bool r_button_pressed = false;
  
  if (digitalRead(L_BUTTON_PIN) == LOW) {
    if (!l_button_debounce) {
      l_button_debounce = true;
      l_button_pressed = true;
    }
  } else {
    l_button_debounce = false;
  }

  if (digitalRead(R_BUTTON_PIN) == LOW) {
    if (!r_button_debounce) {
      r_button_debounce = true;
      r_button_pressed = true;
    }
  } else {
    r_button_debounce = false;
  }
  
  watch.update_screen(l_button_pressed, r_button_pressed, analogRead(A0));

  delay(100);
}
