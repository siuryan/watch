#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

class MessageScreen : public Screen {

    public:
    void update(WatchSystem *watch_system) override;
    void update(WatchSystem *watch_system,
                bool button1_pressed, bool button2_pressed, int pot_pos) override;

};

void MessageScreen::update(WatchSystem *watch_system) {
    // text display tests
    Adafruit_SSD1306 *display = watch_system->get_display();

    display->setTextSize(1);
    display->setTextColor(WHITE);

    display->setCursor(0, 0);
    display->println(F("Notifications"));

    int num_messages = watch_system->get_num_stored_messages();
    if (num_messages == 0) {
        display->println();
        display->println(F("No new notifications"));
        display->display();
        return;
    }

    char *msg_arr[num_messages];
    watch_system->get_messages(msg_arr, num_messages);

    for (int i = 0; i < num_messages; ++i) {
        display->setCursor(0, i*10 + 15);
        display->println(msg_arr[i]);
    }

    display->display();
}

void MessageScreen::update(WatchSystem *watch_system,
                        bool button1_pressed, bool button2_pressed, int pot_pos) {
    update(watch_system);
}
