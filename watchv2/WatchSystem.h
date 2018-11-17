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

class WatchSystem {

    private:
    static const int MAX_MESSAGES = 3;
    static const char *DATETIME_FORMAT;

    /* Software SPI */
    /*
    static const int OLED_MOSI = 9;
    static const int OLED_CLK = 10;
    static const int OLED_DC = 11;
    static const int OLED_CS = 12;
    static const int OLED_RESET = 13;
    */

    // I2C
    static const int OLED_RESET = 4;

    static const int ACCEL_PIN = 8;

    QueueArray<char *> messages;
    int num_messages;
    bool viewed_messages;

    Adafruit_SSD1306 display;
    time_t time_now;
    BluetoothConnection bt_conn;
    ADXL362 xl;

    void init_accel();

    public:
    WatchSystem(Stream &serial);
    ~WatchSystem();

    QueueArray<char *> * get_messages();
    void get_messages(char **arr, int num_messages);
    int get_num_messages();
    int get_num_stored_messages();
    Adafruit_SSD1306 * get_display();
    time_t get_time_now();
    void get_accel_data(int *position);
    bool has_viewed_messages();
    void set_viewed_messages(bool vm);

    void clear_messages();
    void update_time();

    void check_bluetooth();
};
