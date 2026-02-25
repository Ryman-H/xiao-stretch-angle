# Calibration

The sketch converts resistance (kΩ) → angle (deg) using:

angle = a0 + a1*x + a2*x^2 + a3*x^3

where x is resistance in kΩ.

The input is clamped to:
- R_MIN_KOHM = 0.65
- R_MAX_KOHM = 2.35

Update coefficients in `resistanceToAngleDeg()` when recalibrating.
