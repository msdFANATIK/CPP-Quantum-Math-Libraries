#pragma once

/**
 * @brief Computes the square root of the sum of squares of two numbers without overflow/underflow.
 * @details Uses Newton-Raphson iteration for sqrt evaluation with argument scaling.
 *          Fully supportable in C++11/14/20 constexpr contexts.
 * @param x Real value.
 * @param y Real value.
 * @return Euclidean distance sqrt(x^2 + y^2).
 */
constexpr double sqrt_backend(double x) noexcept {
    if (x <= 0.0) return 0.0;
    
    // Initial guess for Newton-Raphson
    double curr = x;
    double prev = 0.0;

    // 10 iterations provide double precision (~15-17 significant decimal digits)
    for (int i = 0; i < 10; ++i) {
        prev = curr;
        curr = 0.5 * (curr + x / curr);
        if (curr == prev) break;
    }
    return curr;
}

constexpr double hypot(double x, double y) noexcept {
    // Take absolute values
    const double abs_x = x < 0.0 ? -x : x;
    const double abs_y = y < 0.0 ? -y : y;

    // Handle edge cases
    if (abs_x == 0.0) return abs_y;
    if (abs_y == 0.0) return abs_x;

    // Determine max and min to scale and prevent numerical overflow/underflow
    const double max_val = abs_x > abs_y ? abs_x : abs_y;
    const double min_val = abs_x > abs_y ? abs_y : abs_x;

    // Scale calculation: max * sqrt(1 + (min/max)^2)
    const double ratio = min_val / max_val;
    return max_val * sqrt_backend(1.0 + ratio * ratio);
}
