#include <Arduino.h>

class Button {

    public:
    Button(int pin);
    bool is_pressed();

    private:
    bool debounce;
    int pin;

};
