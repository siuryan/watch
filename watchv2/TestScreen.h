#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

class TestScreen : public Screen {

    public:
    void update(WatchSystem *watch_system) override;
    void update(WatchSystem *watch_system,
                bool button1_pressed, bool button2_pressed, int pot_pos) override;

};

void TestScreen::update(WatchSystem *watch_system) {
    Adafruit_SSD1306 *display = watch_system->get_display();
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(0, 0);

    double position[3];
    watch_system->get_accel_data(position);
    display->println(position[0]);
    display->println(position[1]);
    display->println(position[2]);
    display->println(sqrt(position[0]*position[0] + position[1]*position[1] + position[2]*position[2]));
    display->println((position[2] < 8 || position[2] > 11) || sqrt(position[0]*position[0] + position[1]*position[1] + position[2]*position[2]) > 12);

    display->display();
}

void TestScreen::update(WatchSystem *watch_system,
            bool button1_pressed, bool button2_pressed, int pot_pos) {
    update(watch_system);
}
