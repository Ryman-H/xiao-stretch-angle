#include <Arduino.h>

const int   ADC_PIN   = A0;
const int   ADC_MAX   = 4095;
const float R_FIXED   = 330000.0f;

const int   OVERSAMPLE_N = 8;
const float SMOOTH_ALPHA = 0.20f;
const uint32_t PRINT_HZ  = 100;

const float R_MIN_KOHM = 0.65f;
const float R_MAX_KOHM = 2.35f;

uint32_t nextPrintUs = 0;

static inline float readAdcAveraged() {
  uint32_t sum = 0;
  for (int i = 0; i < OVERSAMPLE_N; ++i) {
    sum += analogRead(ADC_PIN);
  }
  return sum / (float)OVERSAMPLE_N;
}

static inline float adcToKohm(float adc) {
  if (adc < 1.0f) adc = 1.0f;
  if (adc > (ADC_MAX - 1)) adc = ADC_MAX - 1;
  float r_ohm = R_FIXED * (adc / (ADC_MAX - adc));
  return r_ohm / 1000.0f;
}

static inline float resistanceToAngleDeg(float x) {
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

void setup() {
  Serial.begin(921600);
  while (!Serial) delay(10);  // Wait for USB CDC
  analogReadResolution(12);
}

void loop() {
  float adc = readAdcAveraged();
  float r_kohm = adcToKohm(adc);

  static float r_kohm_smoothed = 0.0f;
  static bool initialized = false;
  if (!initialized) {
    r_kohm_smoothed = r_kohm;
    initialized = true;
  } else {
    r_kohm_smoothed = SMOOTH_ALPHA * r_kohm +
                      (1.0f - SMOOTH_ALPHA) * r_kohm_smoothed;
  }

  float angle_deg = resistanceToAngleDeg(r_kohm_smoothed);

  uint32_t now = micros();
  if ((int32_t)(now - nextPrintUs) >= 0) {
    Serial.print(angle_deg, 3);
    Serial.print(", ");
    Serial.println(r_kohm_smoothed, 4);
    nextPrintUs = now + (1000000UL / PRINT_HZ);
  }
}
