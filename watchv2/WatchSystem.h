#ifndef ADAFRUIT_H
#define ADAFRUIT_H
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

#ifndef TIME_H
#define TIME_H
    #include <Time.h>
#endif

#ifndef BLUETOOTH_CONN_H
#define BLUETOOTH_CONN_H
    #include "BluetoothConnection.h"
#endif

#ifndef QUEUE_H
#define QUEUE_H
    #include "QueueArray.h"
#endif

#ifndef ADXL_H
#define ADXL_h
    #include <ADXL362.h>
#endif

#ifndef VIBR_MOTOR_H
#define VIBR_MOTOR_H
    #include "VibrationMotor.h"
#endif

class WatchSystem {

    private:
    static const byte MAX_MESSAGES = 3;
    static const char *DATETIME_FORMAT;

    // I2C
    static const byte OLED_RESET = 4;

    static const byte ACCEL_PIN = 9;
    static const byte VIBR_MOTOR_PIN = 6;

    QueueArray<char *> messages;
    byte num_messages;
    bool viewed_messages;

    Adafruit_SSD1306 display;
    time_t time_now;
    BluetoothConnection bt_conn;
    ADXL362 xl;
    VibrationMotor vm;

    public:
    WatchSystem();
    ~WatchSystem();

    QueueArray<char *> * get_messages();
    void get_messages(char **arr, byte num_messages);
    byte get_num_messages();
    byte get_num_stored_messages();
    Adafruit_SSD1306 * get_display();
    time_t get_time_now();
    void get_accel_data(double *position);
    bool has_viewed_messages();
    void set_viewed_messages(bool vm);

    void init_accel();
    void clear_messages();
    void update_time();

    void check_bluetooth();
};
