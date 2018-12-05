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
    static const byte MAX_SCREENS = 5;

    static const byte L_BUTTON_PIN = 8;
    static const byte R_BUTTON_PIN = 7;

    static const byte POT_PIN = A0;
    static const short int POT_MAX = 1050;

    Screen *screens[MAX_SCREENS];
    byte current_screen;
    byte num_screens;

    WatchSystem watch_system;

    Button l_button;
    Button r_button;

    void check_bluetooth();
    void add_screen(Screen *screen);
    void switch_screen(byte new_screen);
    bool should_sleep(double *position);

    void update_screen();
    void update_screen(bool button1_pressed, bool button2_pressed, short int pot_pos);
};
