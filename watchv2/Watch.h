#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

#ifndef WATCH_SYSTEM_H
#define WATCH_SYSTEM_H
    #include "WatchSystem.h"
#endif

#ifndef BUTTON_H
#define BUTTON_H
    #include "Button.h"
#endif

class Watch {
    public:
    Watch();
    ~Watch();
    void init();
    void clear();
    void update();

    private:
    static const int MAX_SCREENS = 5;

    static const int L_BUTTON_PIN = 7;
    static const int R_BUTTON_PIN = 2;

    Screen *screens[MAX_SCREENS];
    int current_screen;
    int num_screens;

    WatchSystem watch_system;

    Button l_button;
    Button r_button;
    int pot_pin;

    void check_bluetooth();
    void add_screen(Screen *screen);
    void switch_screen(int new_screen);
    bool should_sleep(int *position);

    void update_screen();
    void update_screen(bool button1_pressed, bool button2_pressed, int pot_pos);
};
