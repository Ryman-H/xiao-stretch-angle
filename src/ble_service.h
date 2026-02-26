#pragma once

#include <bluefruit.h>

class AngleBleService {
public:
    void begin();
    void update(float angleDeg);
    bool isConnected();

private:
    void startAdvertising();

private:
    BLEService _angleService =
        BLEService("12345678-1234-5678-1234-56789abcdef0");

    BLECharacteristic _angleChar =
        BLECharacteristic("12345678-1234-5678-1234-56789abcdef1");
};
