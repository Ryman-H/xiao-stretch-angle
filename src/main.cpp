#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "sensor.h"

AngleSensor sensor(A0, 330000.0f);

void setup() {
    Serial.begin(921600);
    while (!Serial) delay(10);

    sensor.begin();
}

void loop() {
    sensor.update();

    float angle = sensor.getAngleDeg();
    float resistance = sensor.getResistanceKOhm();

    Serial.print(angle, 3);
    Serial.print(", ");
    Serial.println(resistance, 4);

    delay(10);
}
