#pragma once

#include "../core/constants.hpp"
#include "../core/types.hpp"
#include "atan2.hpp"

namespace qm {

/**
 * @brief Computes the square root of a number at compile-time (constexpr).
 * 
 * @param x Input value.
 * @return Square root of x, or 0.0 if x <= 0.
 */
constexpr double sqrt(double x) noexcept {
    if (x <= 0.0) return 0.0;

    double y = (x > 1.0) ? x : 1.0;

    for (int i = 0; i < 14; ++i) {
        y = 0.5 * (y + x / y);
    }
    return y;
}

/**
 * @brief Computes the inverse square root ($1 / \sqrt{x}$).
 * 
 * @param x Input value.
 * @return Inverse square root of x, or 0.0 if x <= 0.
 */
constexpr double inv_sqrt(double x) noexcept {
    double s = sqrt(x);
    return (s > 0.0) ? (1.0 / s) : 0.0;
}

/**
 * @brief Computes the absolute value of a double.
 */
constexpr double abs(double x) noexcept {
    return x < 0.0 ? -x : x;
}

/**
 * @brief Computes the absolute value (magnitude) of a complex number.
 */
constexpr double abs(const Complex& z) noexcept {
    return qm::sqrt(z.re * z.re + z.im * z.im);
}

/**
 * @brief Computes the sine of an angle in radians using Taylor series expansion.
 */
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

/**
 * @brief Computes the cosine of an angle in radians.
 */
constexpr double cos(double x) noexcept {
    // cos(x) = sin(x + π/2)
    return sin(x + PI * 0.5);
}

/**
 * @brief Computes Euler's formula: $e^{i\theta} = \cos\theta + i \sin\theta$.
 */
constexpr Complex exp_i(double theta) noexcept {
    return {cos(theta), sin(theta)};
}

/** @brief Returns module of a complex number (member function). */
constexpr double Complex::mod() const noexcept {
    return qm::sqrt(re * re + im * im);
}

/** @brief Returns module of a complex number (global function). */
constexpr double mod(const Complex& z) noexcept {
    return qm::sqrt(z.re * z.re + z.im * z.im);
}

/** @brief Returns the smaller of two values. */
constexpr double min(double a, double b) noexcept { return a < b ? a : b; }

/** @brief Returns the larger of two values. */
constexpr double max(double a, double b) noexcept { return a > b ? a : b; }

/**
 * @brief Computes base raised to an integer exponent (base^exp).
 */
constexpr double pow(double base, int exp) noexcept {
    if (exp == 0) return 1.0;
    if (base == 0.0) return 0.0;
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

/**
 * @brief Computes the argument (phase angle) of a complex number in radians.
 */
constexpr double arg(const Complex& z) noexcept { 
    return atan2(z.im, z.re); 
}

/**
 * @brief Remainder of the division operation (fmod).
 */
constexpr double fmod(double x, double y) noexcept {
    if (y == 0.0) return 0.0;

    const double abs_x = x < 0.0 ? -x : x;
    const double abs_y = y < 0.0 ? -y : y;

    if (abs_x < abs_y) return x;

    const long long quotient = static_cast<long long>(x / y);
    const double rem = x - static_cast<double>(quotient) * y;

    return rem;
}

/**
 * @brief Rounds down to the largest integer less than or equal to x.
 */
constexpr double floor(double x) noexcept {
    const long long i = static_cast<long long>(x);
    if (x < 0.0 && x != static_cast<double>(i)) {
        return static_cast<double>(i - 1);
    }
    return static_cast<double>(i);
}

/**
 * @brief Rounds up to the smallest integer greater than or equal to x.
 */
constexpr double ceil(double x) noexcept {
    const long long i = static_cast<long long>(x);
    if (x > 0.0 && x != static_cast<double>(i)) {
        return static_cast<double>(i + 1);
    }
    return static_cast<double>(i);
}

} // namespace qm
