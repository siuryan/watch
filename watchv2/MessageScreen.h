#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

class MessageScreen : public Screen {

    public:
    MessageScreen();
    void update(WatchSystem *watch_system) override;
    void update(WatchSystem *watch_system,
                bool button1_pressed, bool button2_pressed, int pot_pos) override;

    private:
    int current_msg;

};

MessageScreen::MessageScreen() : current_msg(0) {}

void MessageScreen::update(WatchSystem *watch_system) {
    // text display tests
    Adafruit_SSD1306 *display = watch_system->get_display();

    watch_system->set_viewed_messages(true);

    display->setTextSize(1);
    display->setTextColor(WHITE);

    display->setCursor(0, 0);
    display->println(F("Notifications"));

    byte num_messages = watch_system->get_num_stored_messages();
    if (num_messages == 0) {
        display->println();
        display->println(F("No new notifications"));
        display->display();
        return;
    }

    char *msg_arr[num_messages];
    watch_system->get_messages(msg_arr, num_messages);

    display->setCursor(0, 25);
    display->println(msg_arr[current_msg]);

    display->drawLine(0, display->height()-10, display->width(), display->height()-10, WHITE);
    display->drawLine(display->width()/3, display->height()-10, display->width()/3, display->height(), WHITE);
    display->drawLine(display->width()*2/3, display->height()-10, display->width()*2/3, display->height(), WHITE);

    display->setCursor(0, display->height() - 7);
    display->print(F("Clear"));

    display->setCursor(display->width()/2 - 9, display->height() - 7);
    display->print(String(current_msg + 1) + F("/") + String(num_messages));

    display->setCursor(display->width() - 25, display->height() - 7);
    if (num_messages > 1) {
        display->print(F("Next"));
    }

    display->display();
}

void MessageScreen::update(WatchSystem *watch_system,
                        bool button1_pressed, bool button2_pressed, int pot_pos) {
    if (button1_pressed) {
        watch_system->clear_messages();
        current_msg = 0;
    }
    if (button2_pressed) {
        ++current_msg %= watch_system->get_num_stored_messages();
    }

    update(watch_system);
}
