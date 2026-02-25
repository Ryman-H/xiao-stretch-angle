# XIAO nRF52840 Sense Stretch Sensor → Angle (Arduino IDE)

Reads a stretch sensor on A0 (voltage divider), applies EMA smoothing, converts resistance (kΩ) to angle (deg) using a cubic calibration, and prints:

`angle_deg, resistance_kohm`

Designed for Arduino Serial Plotter.

## Hardware
- Seeed Studio XIAO nRF52840 Sense
- Fixed resistor: 330 kΩ
- Stretch sensor

## Wiring (default)
3.3V → 330kΩ → A0 → stretch sensor → GND

## Run (Arduino IDE)
1. Install XIAO nRF52840 Sense board support (Seeed nRF52 boards).
2. Open: `sketch/XIAO_StretchAngle/XIAO_StretchAngle.ino`
3. Select board: **Seeed XIAO nRF52840 Sense**
4. Upload
5. Tools → Serial Plotter
   - Baud: **921600**

## GitHub CI/CD
- CI compiles the sketch on every push/PR using Arduino CLI.
- Release workflow builds and attaches compiled outputs when you create a tag like `v1.0.0`.

See `.github/workflows/`.
