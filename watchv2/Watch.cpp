#include "Watch.h"
#include "HomeScreen.h"
#include "TestScreen.h"
#include "MessageScreen.h"

#include <math.h>

Watch::Watch(Stream &serial) :
    current_screen(0), num_screens(0), watch_system(serial) {}

Watch::~Watch() {
    delete[] screens;
}

void Watch::init() {
    Adafruit_SSD1306 *display = watch_system.get_display();
    display->begin(SSD1306_SWITCHCAPVCC);

    display->display();

    add_screen(new HomeScreen());
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

bool Watch::should_sleep(int *position) {
    return sqrt(position[0]*position[0] + position[1]*position[1] + position[2]*position[2]) < 3;
}

void Watch::update() {
    clear();

    int position[3];
    watch_system.get_accel_data(position);
    bool sleep = should_sleep(position);

    while (sleep) {
        sleep = should_sleep(position);
        delay(1000);
    }

    watch_system.update_time();
    watch_system.check_bluetooth();
}

void Watch::update_screen() {
    update();
    screens[current_screen]->update(&watch_system);
}

void Watch::update_screen(bool button1_pressed, bool button2_pressed, int pot_pos) {
    update();

    int screen = map(pot_pos, 0, 667, 0, num_screens);
    if (screen <= 0 || screen >= num_screens) screen == 0;
    switch_screen(screen);

    screens[current_screen]->update(&watch_system, button1_pressed, button2_pressed, pot_pos);
}

void Watch::switch_screen(int new_screen) {
    new_screen %= MAX_SCREENS;
    current_screen = new_screen;
}
