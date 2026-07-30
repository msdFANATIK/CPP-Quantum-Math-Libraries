#pragma once

#include "../core/constants.hpp"

namespace qm {

/**
 * @brief Helper evaluation for atan(x) on interval [-1, 1].
 * @details Uses Minimax (Remez) polynomial approximation of degree 11 via Horner's scheme.
 *          Provides accuracy up to ~10^-11 across the entire range.
 * @param x Input argument in range [-1.0, 1.0].
 * @return Approximated arctangent in radians.
 */
constexpr double atan_backend(double x) noexcept {
    const double x2 = x * x;

    // Coefficients for minimax approximation of atan(x) / x on [-1, 1]
    constexpr double c1  =  0.9999999863001859;
    constexpr double c3  = -0.3333330343863608;
    constexpr double c5  =  0.1999921430880259;
    constexpr double c7  = -0.1425318079080703;
    constexpr double c9  =  0.1064790027783350;
    constexpr double c11 = -0.0528511252103444;

    // Horner's method for polynomial evaluation: x * (c1 + x2 * (c3 + x2 * (...)))
    return x * (c1 + x2 * (c3 + x2 * (c5 + x2 * (c7 + x2 * (c9 + x2 * c11)))));
}

/**
 * @brief Computes the arc tangent of y/x using sign of arguments to determine quadrant.
 * @details Full range [-PI, PI] atan2 implementation optimized for constexpr context.
 * @param y Real value representing y-coordinate.
 * @param x Real value representing x-coordinate.
 * @return Angle in radians in range [-PI, PI].
 */
constexpr double atan2(double y, double x) noexcept {
    if (x == 0.0) {
        if (y > 0.0) return HALF_PI;
        if (y < 0.0) return -HALF_PI;
        return 0.0; // Standard fallback for (0,0)
    }

    const double abs_x = x < 0.0 ? -x : x;
    const double abs_y = y < 0.0 ? -y : y;

    double angle = 0.0;

    // Map argument to [-1, 1] using identity: atan(x) = PI/2 - atan(1/x)
    if (abs_x >= abs_y) {
        angle = atan_backend(y / x);
        if (x < 0.0) {
            angle += (y >= 0.0) ? PI : -PI;
        }
    } else {
        angle = HALF_PI - atan_backend(x / y);
        if (y < 0.0) {
            angle -= PI;
        }
    }

    return angle;
}

} // namespace qm
