#pragma once

namespace qm {

// ---------------------------------------------------------------------
// Mathematical Constants
// ---------------------------------------------------------------------
constexpr double PI         = 3.14159265358979323846;
constexpr double HALF_PI    = 1.57079632679489661923;
constexpr double TWO_PI     = 6.28318530717958647692;
constexpr double SQRT2      = 1.41421356237309504880;
constexpr double INV_SQRT2  = 0.70710678118654752440;   // 1/√2
constexpr double SQRT3      = 1.73205080756887729352;
constexpr double E          = 2.71828182845904523536;

// ---------------------------------------------------------------------
// Quantum Mechanics & Physics Constants (SI Units)
// ---------------------------------------------------------------------
constexpr double HBAR       = 1.054571817e-34;          // Reduced Planck constant (J·s)
constexpr double PLANCK_H   = 6.62607015e-34;           // Planck constant (J·s)
constexpr double ELEC_MASS  = 9.1093837015e-31;         // Electron mass (kg)
constexpr double ELEC_CHARGE= 1.602176634e-19;          // Elementary charge (C)

// ---------------------------------------------------------------------
// Numerical Tolerances & Limits
// ---------------------------------------------------------------------
constexpr double EPSILON    = 1e-12;                    // Zero-threshold for complex float comparisons
constexpr double NORM_TOL   = 1e-9;                     // Tolerance for state vector normalization check

} // namespace qm
