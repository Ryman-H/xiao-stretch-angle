// XIAO nRF52840 Sense + stretch sensor (calibrated)
// Outputs: "angle_deg, resistance_kohm" per line for Serial Plotter
//
// Wiring assumed (DEFAULT):
//   3.3V -> R_FIXED (330k) -> A0 -> Stretch sensor -> GND
//
// Units: resistance in kΩ (kilo-ohms); angle in degrees.
// Calibration model: cubic polynomial (resistance_kohm -> angle_deg).

#include <Arduino.h>

// ADC / sensor
const int   ADC_PIN   = A0;
const int   ADC_MAX   = 4095;        // 12-bit ADC
const float R_FIXED   = 330000.0f;   // ohms

// Sampling + smoothing
const int   OVERSAMPLE_N = 8;        // ADC reads per averaged sample
const float SMOOTH_ALPHA = 0.20f;    // EMA alpha: 0.0..1.0
const uint32_t PRINT_HZ  = 100;      // lines/sec to Serial Plotter

// Clamp range for resistance (kΩ)
const float R_MIN_KOHM = 0.65f;
const float R_MAX_KOHM = 2.35f;

uint32_t nextPrintUs = 0;

void setup() {
  Serial.begin(921600);
  analogReadResolution(12);
  delay(10);
}

// Read averaged ADC value (oversampling)
static inline float readAdcAveraged() {
  uint32_t sum = 0;
  for (int i = 0; i < OVERSAMPLE_N; ++i) {
    sum += analogRead(ADC_PIN);
  }
  return sum / (float)OVERSAMPLE_N;
}

// Convert averaged ADC reading -> resistance in kΩ, for divider:
// 3.3V -> R_FIXED -> A0 -> SENSOR -> GND
// R_sensor = R_fixed * adc / (ADCmax - adc)
static inline float adcToKohm(float adc) {
  if (adc < 1.0f) adc = 1.0f;
  if (adc > (ADC_MAX - 1)) adc = ADC_MAX - 1;
  float r_ohm = R_FIXED * (adc / (ADC_MAX - adc));
  return r_ohm / 1000.0f; // kΩ
}

// Calibrated cubic model: resistance_kohm -> angle_deg
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

void loop() {
  float adc = readAdcAveraged();
  float r_kohm = adcToKohm(adc);

  // EMA smoothing
  static float r_kohm_smoothed = 0.0f;
  static bool initialized = false;
  if (!initialized) {
    r_kohm_smoothed = r_kohm;
    initialized = true;
  } else {
    r_kohm_smoothed = SMOOTH_ALPHA * r_kohm + (1.0f - SMOOTH_ALPHA) * r_kohm_smoothed;
  }

  float angle_deg = resistanceToAngleDeg(r_kohm_smoothed);

  // Print for Serial Plotter: "angle_deg, resistance_kohm"
  uint32_t now = micros();
  if ((int32_t)(now - nextPrintUs) >= 0) {
    Serial.print(angle_deg, 3);
    Serial.print(", ");
    Serial.println(r_kohm_smoothed, 4);
    nextPrintUs = now + (1000000UL / PRINT_HZ);
  }
}
