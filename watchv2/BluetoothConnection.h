#include "Stream.h"

#ifndef S_SERIAL_H
#define S_SERIAL_H
    #include <SoftwareSerial.h>
#endif

class BluetoothConnection {

    public:
    static const int MAX_MESSAGE_SIZE = 32;

    BluetoothConnection();
    int get_message(char *message);
    void send_message(char *message);

    private:
    static const int SS_RX_PIN = 10;
    static const int SS_TX_PIN = 11;

    SoftwareSerial serial;

};
