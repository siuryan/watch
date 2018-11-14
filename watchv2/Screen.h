#ifndef ADAFRUIT_H
#define ADAFRUIT_H
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

#ifndef TIME_H
#define TIME_H
    #include <Time.h>
#endif

#ifndef QUEUE_H
#define QUEUE_H
    #include <QueueArray.h>
#endif

#ifndef WATCH_SYSTEM_H
#define WATCH_SYSTEM_H
    #include "WatchSystem.h"
#endif

class Screen {
    public:
    virtual void update(WatchSystem *watch_system) = 0;
    virtual void update(WatchSystem *watch_system, bool button1_pressed, bool button2_pressed, int pot_pos) = 0;
};
