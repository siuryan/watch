#include "Button.h"

Button::Button(int pin) : debounce(false), pin(pin) {
    pinMode(pin, INPUT_PULLUP);
}

bool Button::is_pressed() {
    if (digitalRead(pin) == LOW) {
        if (!debounce) {
            debounce = true;
            return true;
        }
    } else {
        debounce = false;
    }

    return false;
}
