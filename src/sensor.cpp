#include "sensor.h"

AngleSensor::AngleSensor(int adcPin, float fixedResistorOhm)
    : _adcPin(adcPin), _rFixed(fixedResistorOhm) {}

void AngleSensor::begin() {
    analogReadResolution(12);
}

float AngleSensor::readAdcAveraged() {
    uint32_t sum = 0;
    for (int i = 0; i < OVERSAMPLE_N; ++i) {
        sum += analogRead(_adcPin);
    }
    return sum / (float)OVERSAMPLE_N;
}

float AngleSensor::adcToKohm(float adc) {
    if (adc < 1.0f) adc = 1.0f;
    if (adc > (ADC_MAX - 1)) adc = ADC_MAX - 1;

    float r_ohm = _rFixed * (adc / (ADC_MAX - adc));
    return r_ohm / 1000.0f;
}

float AngleSensor::resistanceToAngleDeg(float x) const {
    if (x < R_MIN_KOHM) x = R_MIN_KOHM;
    if (x > R_MAX_KOHM) x = R_MAX_KOHM;

    const float a0 = 393.09791316f;
    const float a1 = -638.11199115f;
    const float a2 =  359.40016819f;
    const float a3 =  -67.94599851f;

    float x2 = x * x;
    float x3 = x2 * x;
    return a0 + a1 * x + a2 * x2 + a3 * x3;
}

void AngleSensor::update() {
    float adc = readAdcAveraged();
    float r_kohm = adcToKohm(adc);

    if (!_initialized) {
        _rKohmSmoothed = r_kohm;
        _initialized = true;
    } else {
        _rKohmSmoothed =
            SMOOTH_ALPHA * r_kohm +
            (1.0f - SMOOTH_ALPHA) * _rKohmSmoothed;
    }
}

float AngleSensor::getResistanceKOhm() const {
    return _rKohmSmoothed;
}

float AngleSensor::getAngleDeg() const {
    return resistanceToAngleDeg(_rKohmSmoothed);
}
