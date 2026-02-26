#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "sensor.h"
#include "ble_service.h"

AngleSensor sensor(A0, 330000.0f);
AngleBleService ble;

void setup() {

    Serial.begin(921600);
    while (!Serial) delay(10);

    Serial.println("XIAO BLE Angle Sensor");

    sensor.begin();
    ble.begin();
}

void loop() {

    sensor.update();

    float angle = sensor.getAngleDeg();
    float resistance = sensor.getResistanceKOhm();

    // Serial output
    Serial.print(angle, 3);
    Serial.print(", ");
    Serial.println(resistance, 4);

    // BLE update
    ble.update(angle);

    delay(50);  // 20Hz update
}
