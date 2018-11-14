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
    display->println(F("Hello, world!"));
    display->setTextColor(BLACK, WHITE); // 'inverted' text
    display->println(3.141592);
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->print(F("0x"));
    display->println(0xDEADBEEF, HEX);
    display->display();
}

void TestScreen::update(WatchSystem *watch_system,
            bool button1_pressed, bool button2_pressed, int pot_pos) {
    update(watch_system);
}
