class VibrationMotor {

    public:
    VibrationMotor(byte pin) : pin(pin) {
        pinMode(pin, OUTPUT);
    }

    void vibrate(int time_millis) {
        digitalWrite(pin, HIGH);
        delay(time_millis);
        digitalWrite(pin, LOW);
    }

    private:
    byte pin;

};
