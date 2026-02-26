#pragma once

#include <Arduino.h>

class AngleSensor {
public:
    AngleSensor(int adcPin, float fixedResistorOhm);

    void begin();
    void update();

    float getResistanceKOhm() const;
    float getAngleDeg() const;

private:
    float readAdcAveraged();
    float adcToKohm(float adc);
    float resistanceToAngleDeg(float r_kohm);

private:
    int _adcPin;
    float _rFixed;

    float _rKohmSmoothed = 0.0f;
    bool _initialized = false;

    static constexpr int   ADC_MAX = 4095;
    static constexpr int   OVERSAMPLE_N = 8;
    static constexpr float SMOOTH_ALPHA = 0.20f;

    static constexpr float R_MIN_KOHM = 0.65f;
    static constexpr float R_MAX_KOHM = 2.35f;
};
