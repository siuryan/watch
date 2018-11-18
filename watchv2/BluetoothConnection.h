#include "Stream.h"

#ifndef S_SERIAL_H
#define S_SERIAL_H
    #include <SoftwareSerial.h>
#endif

class BluetoothConnection {

    public:
    static const uint8_t MAX_MESSAGE_SIZE = 32;

    BluetoothConnection();
    uint8_t get_message(char *message);
    void send_message(char *message);

    private:
    static const uint8_t SS_RX_PIN = 10;
    static const uint8_t SS_TX_PIN = 11;

    SoftwareSerial serial;

};
