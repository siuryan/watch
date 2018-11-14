#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

#ifndef WATCH_SYSTEM_H
#define WATCH_SYSTEM_H
    #include "WatchSystem.h"
#endif

class Watch {
    public:
    Watch(Stream &serial);
    ~Watch();
    void init();
    void clear();
    void update_screen();
    void update_screen(bool button1_pressed, bool button2_pressed, int pot_pos);

    private:
    static const int MAX_SCREENS = 5;

    Screen *screens[MAX_SCREENS];
    int current_screen;
    int num_screens;

    WatchSystem watch_system;

    void update();
    void check_bluetooth();
    void add_screen(Screen *screen);
    void switch_screen(int new_screen);
    bool should_sleep(int *position);
};
