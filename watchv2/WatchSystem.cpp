#include "WatchSystem.h"
#include <stdio.h>

const char *WatchSystem::DATETIME_FORMAT = "%d/%d/%d %d:%d:%d";

WatchSystem::WatchSystem()
    : num_messages(0), viewed_messages(false),
      display(OLED_RESET),
      time_now(0), bt_conn(), vm(VIBR_MOTOR_PIN), health() {
}

void WatchSystem::init_accel() {
    xl.begin(ACCEL_PIN);
    xl.beginMeasure();
}

WatchSystem::~WatchSystem() {
    clear_messages();
}

QueueArray<char *> * WatchSystem::get_messages() {
    return &messages;
}

void WatchSystem::get_messages(char **arr, byte num_messages) {
    messages.toArray(arr, num_messages);
}

byte WatchSystem::get_num_messages() {
    return num_messages;
}

byte WatchSystem::get_num_stored_messages() {
    return messages.count();
}

Adafruit_SSD1306 * WatchSystem::get_display() {
    return &display;
}

time_t WatchSystem::get_time_now() {
    return time_now;
}

bool WatchSystem::has_viewed_messages() {
    return viewed_messages;
}

void WatchSystem::set_viewed_messages(bool vm) {
    viewed_messages = vm;
}

Health * WatchSystem::get_health() {
    return &health;
}

void WatchSystem::clear_messages() {
    while (!messages.isEmpty()) {
        delete messages.pop();
    }
    num_messages = 0;
}

void WatchSystem::update_time() {
    time_now = now();
    if (now() < 1) time_now = 1;
}

void WatchSystem::check_bluetooth() {
    char *message = new char[bt_conn.MAX_MESSAGE_SIZE];
    byte msg_status = bt_conn.get_message(message);

    switch (msg_status) {

    case 0:
        delete message;
        break;

    case 1:
        if (messages.count() >= MAX_MESSAGES) {
            delete messages.pop();
        }

        messages.push(message);
        ++num_messages;
        viewed_messages = false;
        vm.vibrate(400);
        break;

    case 2:
        int year, month, day, hour, minute, second;
        sscanf(message, DATETIME_FORMAT, &year, &month, &day, &hour, &minute, &second);
        setTime(hour, minute, second, day, month, year);
        update_time();
        delete message;
        break;

    default:
        delete message;
        break;
    }
}

void WatchSystem::get_accel_data(double *position) {
    int16_t x, y, z, t;
    xl.readXYZTData(x, y, z, t);
    position[0] = x / 100.0;
    position[1] = y / 100.0;
    position[2] = z / 100.0;
    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.print("\t");
    Serial.println(z);
}

void WatchSystem::update_health(const double *position) {
    health.update_footsteps(position);
}
