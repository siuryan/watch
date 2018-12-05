#include "Watch.h"
#include "HomeScreen.h"
#include "TestScreen.h"
#include "MessageScreen.h"

#include <math.h>
#include <Arduino.h>

Watch::Watch() :
    current_screen(0), num_screens(0), watch_system(),
    l_button(L_BUTTON_PIN), r_button(R_BUTTON_PIN) {
}

Watch::~Watch() {
    delete[] screens;
}

void Watch::init() {
    Adafruit_SSD1306 *display = watch_system.get_display();
    display->begin(SSD1306_SWITCHCAPVCC);

    display->display();
    watch_system.init_accel();

    add_screen(new HomeScreen());
    //add_screen(new TestScreen());
    add_screen(new MessageScreen());
}

void Watch::add_screen(Screen *screen) {
    if (num_screens < MAX_SCREENS) {
        screens[num_screens] = screen;
        num_screens++;
    }
}

void Watch::clear() {
    watch_system.get_display()->clearDisplay();
}

bool Watch::should_sleep(double *position) {
    return (position[2] < 8 || position[2] > 11)
        || sqrt(position[0]*position[0] + position[1]*position[1] + position[2]*position[2]) > 12;
}

void Watch::update() {
    clear();

    double position[3];
    watch_system.get_accel_data(position);
    bool sleep = should_sleep(position);
    watch_system.get_display()->dim(sleep);

    watch_system.update_time();
    watch_system.check_bluetooth();

    update_screen(l_button.is_pressed(), r_button.is_pressed(), analogRead(POT_PIN));
}

void Watch::update_screen() {
    screens[current_screen]->update(&watch_system);
}

void Watch::update_screen(bool button1_pressed, bool button2_pressed, short int pot_pos) {
    short int screen = map(pot_pos, 0, POT_MAX, 0, num_screens);
    if (screen <= 0 || screen >= num_screens) screen == 0;
    switch_screen(screen);

    screens[current_screen]->update(&watch_system, button1_pressed, button2_pressed, pot_pos);
}

void Watch::switch_screen(byte new_screen) {
    new_screen %= MAX_SCREENS;
    current_screen = new_screen;
}
