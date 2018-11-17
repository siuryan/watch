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

    int num_messages = watch_system->get_num_stored_messages();
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

    // TODO: display arrows

    display->display();
}

void MessageScreen::update(WatchSystem *watch_system,
                        bool button1_pressed, bool button2_pressed, int pot_pos) {
    if (button1_pressed) {
        --current_msg %= watch_system->get_num_stored_messages();
    }
    if (button2_pressed) {
        ++current_msg %= watch_system->get_num_stored_messages();
    }

    update(watch_system);
}
