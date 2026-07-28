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

    for (int i = 0; i < 12; ++i) {
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
    const double two_pi = 2.0 * PI;
    long long k = static_cast<long long>(x / two_pi);
    x -= k * two_pi;

    while (x > PI)  x -= two_pi;
    while (x < -PI) x += two_pi;

    const double x2 = x * x;
    double term = x;
    double result = x;

    term *= -x2 / (2.0 * 3.0);
    result += term;
    term *= -x2 / (4.0 * 5.0);
    result += term;
    term *= -x2 / (6.0 * 7.0);
    result += term;
    term *= -x2 / (8.0 * 9.0);
    result += term;
    term *= -x2 / (10.0 * 11.0);
    result += term;
    term *= -x2 / (12.0 * 13.0);
    result += term;

    return result;
}

inline double cos(double x) noexcept {
    return sin(x + PI * 0.5);
}

// e^{iθ} = cosθ + i sinθ
inline Complex exp_i(double theta) noexcept {
    return {cos(theta), sin(theta)};
}

} // namespace qm
