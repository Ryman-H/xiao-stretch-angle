#include "ble_service.h"

void AngleBleService::begin() {

    Bluefruit.begin();
    Bluefruit.setTxPower(4);
    Bluefruit.setName("XIAO-Angle");

    _angleService.begin();

    _angleChar.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
    _angleChar.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    _angleChar.setFixedLen(sizeof(float));
    _angleChar.begin();

    startAdvertising();
}

void AngleBleService::startAdvertising() {

    Bluefruit.Advertising.stop();

    Bluefruit.Advertising.addFlags(
        BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addService(_angleService);
    Bluefruit.Advertising.addName();

    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);
    Bluefruit.Advertising.setFastTimeout(30);
    Bluefruit.Advertising.start(0);
}

void AngleBleService::update(float angleDeg) {
    if (Bluefruit.connected()) {
        _angleChar.notify(&angleDeg, sizeof(angleDeg));
    }
}

bool AngleBleService::isConnected() {
    return Bluefruit.connected();
}
