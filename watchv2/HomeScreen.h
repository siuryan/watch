#ifndef SCREEN_H
#define SCREEN_H
    #include "Screen.h"
#endif

class HomeScreen : public Screen {

    public:
    void update(WatchSystem *watch_system) override;
    void update(WatchSystem *watch_system,
                bool button1_pressed, bool button2_pressed, int pot_pos) override;

};

static const float pi = 3.14159;

String zeroFix(int num) {
    if (num < 10) { return "0" + String(num); }
    return String(num);
}

String getTimeString(time_t t) {
    return zeroFix(hour(t)) + F(":") + zeroFix(minute(t)) + F(":") + zeroFix(second(t));
}

void drawClock(Adafruit_SSD1306 *display, byte h, byte m) {
    byte minuteHand = 20;
    byte hourHand = 15;

    byte centerX = 3 * display->width() / 4;
    byte centerY = display->height() / 2;
    byte minuteX = sin(6*m*pi/180) * minuteHand + centerX;
    byte minuteY = -cos(6*m*pi/180) * minuteHand + centerY;
    byte hourX = sin((30*h+0.5*m)*pi/180) * hourHand + centerX;
    byte hourY = -cos(30*h*pi/180) * hourHand + centerY;

    display->drawCircle(centerX, centerY, 25, WHITE);
    display->fillCircle(centerX, centerY, 1, WHITE);
    display->drawLine(centerX, centerY, minuteX, minuteY, WHITE);
    display->drawLine(centerX, centerY, hourX, hourY, WHITE);
}

void HomeScreen::update(WatchSystem *watch_system) {
    // text display tests
    time_t t = watch_system->get_time_now();

    String w = dayStr(weekday(t));
    String M = monthStr(month(t));
    String d = String(day(t));
    byte h = hour(t);
    byte m = minute(t);
    byte s = second(t);

    Adafruit_SSD1306 *display = watch_system->get_display();
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(1, display->height()/2 - 13);
    display->println(w);
    display->setCursor(1, display->height()/2 - 3);
    display->println(M + F(" ") + d);
    display->setCursor(1, display->height()/2 + 7);
    display->println(getTimeString(t));

    drawClock(display, hour(t), minute(t));

    display->setCursor(display->width()-5, 5);
    if (watch_system->get_num_messages() > 0 && !watch_system->has_viewed_messages()) {
        display->println("*");
    }

    display->setCursor(0, display->height()-10);
    display->println(watch_system->get_health()->get_footsteps());

    display->display();
}

void HomeScreen::update(WatchSystem *watch_system,
                        bool button1_pressed, bool button2_pressed, int pot_pos) {
    update(watch_system);
}
