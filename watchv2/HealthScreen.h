#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

class HealthScreen : public Screen {

    public:
    void update(WatchSystem *watch_system) override;
    void update(WatchSystem *watch_system,
                bool button1_pressed, bool button2_pressed, int pot_pos) override;

};

void HealthScreen::update(WatchSystem *watch_system) {
    Adafruit_SSD1306 *display = watch_system->get_display();
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(0, 0);
    display->println(F("Health"));
    display->println(F(" "));
    display->print(watch_system->get_health()->get_footsteps());
    display->println(F(" steps"));

    display->display();
}

void HealthScreen::update(WatchSystem *watch_system,
                        bool button1_pressed, bool button2_pressed, int pot_pos) {
    update(watch_system);
}
