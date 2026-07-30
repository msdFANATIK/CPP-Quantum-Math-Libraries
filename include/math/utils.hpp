#pragma once

#include "../core/constants.hpp"
#include "../core/types.hpp"
#include "atan2.hpp"

namespace qm {

constexpr double sqrt(double x) noexcept {
    if (x <= 0.0) return 0.0;

    double y = (x > 1.0) ? x : 1.0;

    for (int i = 0; i < 14; ++i) {
        y = 0.5 * (y + x / y);
    }
    return y;
}

constexpr double inv_sqrt(double x) noexcept {
    double s = sqrt(x);
    return (s > 0.0) ? (1.0 / s) : 0.0;
}

constexpr double abs(double x) noexcept {
    return x < 0.0 ? -x : x;
}

constexpr double sin(double x) noexcept {
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

constexpr double cos(double x) noexcept {
    // cos(x) = sin(x + π/2)
    return sin(x + PI * 0.5);
}

// e^{iθ} = cosθ + i sinθ
constexpr Complex exp_i(double theta) noexcept {
    return {cos(theta), sin(theta)};
}

constexpr double Complex::mod() const noexcept {
    return qm::sqrt(re * re + im * im);
}

constexpr double mod(const Complex& z) noexcept {
    return qm::sqrt(z.re * z.re + z.im * z.im);
}

constexpr double min(double a, double b) noexcept { return a < b ? a : b; }
constexpr double max(double a, double b) noexcept { return a > b ? a : b; }

constexpr double pow(double base, int exp) noexcept {
    if (exp == 0) return 1.0;
    if (exp < 0) {
        base = 1.0 / base;
        exp = -exp;
    }
    double res = 1.0;
    while (exp > 0) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

constexpr double arg(const Complex& z) noexcept { 
    return atan2(z.im, z.re); 
}

constexpr double fmod(double x, double y) noexcept {
    if (y == 0.0) return 0.0;

    const double abs_x = x < 0.0 ? -x : x;
    const double abs_y = y < 0.0 ? -y : y;

    if (abs_x < abs_y) return x;

    const long long quotient = static_cast<long long>(x / y);
    const double rem = x - static_cast<double>(quotient) * y;

    return rem;
}

constexpr double floor(double x) noexcept {
    const long long i = static_cast<long long>(x);
    if (x < 0.0 && x != static_cast<double>(i)) {
        return static_cast<double>(i - 1);
    }
    return static_cast<double>(i);
}

constexpr double ceil(double x) noexcept {
    const long long i = static_cast<long long>(x);
    if (x > 0.0 && x != static_cast<double>(i)) {
        return static_cast<double>(i + 1);
    }
    return static_cast<double>(i);
}
} // namespace qm
