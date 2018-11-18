#include "BluetoothConnection.h"

BluetoothConnection::BluetoothConnection() : serial(SoftwareSerial(SS_RX_PIN, SS_TX_PIN)) {
    serial.begin(9600);
}

void remove_header(char *message, int size) {
    for (int i = 0; i < size - 1; ++i) {
        message[i] = message[i+1];
    }
}

/**
 * Returns a status
 * 0: no message
 * 1: notif
 * 2: time
 */
int BluetoothConnection::get_message(char *message) {
    if (!serial.available()) {
        return 0;
    }

    int size = 0;
    while (serial.available() && size < MAX_MESSAGE_SIZE - 1) {
        message[size++] = serial.read();
        message[size] = '\0';
    }

    if (size == 0) {
        return 0;
    }
    else {
        if (message[0] == '1') {
            remove_header(message, MAX_MESSAGE_SIZE);
            return 1;
        }
        else if (message[0] == '2') {
            remove_header(message, MAX_MESSAGE_SIZE);
            return 2;
        }
    }

    return 0;
}

void BluetoothConnection::send_message(char *message) {
    serial.println(String(message));
}
