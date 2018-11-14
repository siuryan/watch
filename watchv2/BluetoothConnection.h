#include "Stream.h"

class BluetoothConnection {

    public:
    static const int MAX_MESSAGE_SIZE = 32;

    BluetoothConnection(Stream *serial);
    int get_message(char *message);
    void send_message(char *message);

    private:
    Stream *serial;

};
