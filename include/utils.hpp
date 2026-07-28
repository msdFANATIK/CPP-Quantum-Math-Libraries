#pragma once

#include "constants.hpp"
#include "types.hpp"

namespace qm {

constexpr double abs(double x) noexcept {
    return x < 0.0 ? -x : x;
}

inline double sqrt(double x) noexcept {
    if (x <= 0.0) return 0.0;

    double y = (x > 1.0) ? x : 1.0;

    for (int i = 0; i < 14; ++i) {
        y = 0.5 * (y + x / y);
    }
    return y;
}

inline double abs(const Complex& z) noexcept {
    return sqrt(z.norm_sq());
}

inline double inv_sqrt(double x) noexcept {
    double s = sqrt(x);
    return (s > 0.0) ? (1.0 / s) : 0.0;
}

inline double sin(double x) noexcept {
    constexpr double two_pi = 2.0 * PI;

    x = x - two_pi * static_cast<long long>(x / two_pi + (x >= 0.0 ? 0.5 : -0.5));

    if (x > PI)  x -= two_pi;
    if (x < -PI) x += two_pi;

    bool flip = false;
    if (x > PI * 0.5) {
        x = PI - x;
        flip = true;
    } else if (x < -PI * 0.5) {
        x = -PI - x;
        flip = true;
    }

    const double x2 = x * x;

    double result = x;
    double term = x;

    term *= -x2 / (2.0 * 3.0);   result += term;  // -x³/3!
    term *= -x2 / (4.0 * 5.0);   result += term;  // +x⁵/5!
    term *= -x2 / (6.0 * 7.0);   result += term;
    term *= -x2 / (8.0 * 9.0);   result += term;
    term *= -x2 / (10.0 * 11.0); result += term;
    term *= -x2 / (12.0 * 13.0); result += term;
    term *= -x2 / (14.0 * 15.0); result += term;

    return flip ? -result : result;
}

inline double cos(double x) noexcept {
    // cos(x) = sin(x + π/2)
    return sin(x + PI * 0.5);
}

// e^{iθ} = cosθ + i sinθ
inline Complex exp_i(double theta) noexcept {
    return {cos(theta), sin(theta)};
}

} // namespace qm
