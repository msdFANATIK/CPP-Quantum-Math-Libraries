#pragma once

namespace qm {

/**
 * @brief Lightweight representation of a complex number z = re + i * im.
 *
 * Provides a complete set of arithmetic operations, comparisons, and basic
 * complex-plane utility methods designed for zero-overhead evaluation.
 */
struct Complex {
    double re; ///< Real component of the complex number.
    double im; ///< Imaginary component of the complex number.

    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    /**
     * @brief Constructs a complex number from real and imaginary parts.
     * @param r Real part (defaults to 0.0).
     * @param i Imaginary part (defaults to 0.0).
     */
    constexpr Complex(double r = 0.0, double i = 0.0) noexcept : re(r), im(i) {}

    // ---------------------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------------------

    /** @brief Returns the negation of the complex number (-z). */
    constexpr Complex operator-() const noexcept {
        return {-re, -im};
    }

    // ---------------------------------------------------------------------
    // Binary Arithmetic Operators (Complex - Complex)
    // ---------------------------------------------------------------------

    /** @brief Adds two complex numbers. */
    constexpr Complex operator+(const Complex& o) const noexcept {
        return {re + o.re, im + o.im};
    }

    /** @brief Subtracts two complex numbers. */
    constexpr Complex operator-(const Complex& o) const noexcept {
        return {re - o.re, im - o.im};
    }

    /** @brief Multiplies two complex numbers. */
    constexpr Complex operator*(const Complex& o) const noexcept {
        return {
            re * o.re - im * o.im,
            re * o.im + im * o.re
        };
    }

    /** @brief Divides two complex numbers. */
    constexpr Complex operator/(const Complex& o) const noexcept {
        const double den = o.norm_sq();
        return {
            (re * o.re + im * o.im) / den,
            (im * o.re - re * o.im) / den
        };
    }

    // ---------------------------------------------------------------------
    // Binary Arithmetic Operators (Scalar Operations)
    // ---------------------------------------------------------------------

    /** @brief Multiplies complex number by a scalar (z * s). */
    constexpr Complex operator*(double s) const noexcept {
        return {re * s, im * s};
    }

    /** @brief Multiplies scalar by a complex number (s * z). */
    friend constexpr Complex operator*(double s, const Complex& z) noexcept {
        return z * s;
    }

    /** @brief Divides complex number by a scalar (z / s). */
    constexpr Complex operator/(double s) const noexcept {
        return {re / s, im / s};
    }

    /** @brief Divides a scalar by a complex number (s / z). */
    friend Complex operator/(double s, const Complex& z) noexcept {
        const double den = z.norm_sq();
        return {s * z.re / den, -s * z.im / den};
    }

    // ---------------------------------------------------------------------
    // Compound Assignment Operators
    // ---------------------------------------------------------------------

    /** @brief Adds and assigns another complex number. */
    constexpr Complex& operator+=(const Complex& o) noexcept {
        re += o.re;
        im += o.im;
        return *this;
    }

    /** @brief Subtracts and assigns another complex number. */
    constexpr Complex& operator-=(const Complex& o) noexcept {
        re -= o.re;
        im -= o.im;
        return *this;
    }

    /** @brief Multiplies and assigns another complex number. */
    constexpr Complex& operator*=(const Complex& o) noexcept {
        const double new_re = re * o.re - im * o.im;
        const double new_im = re * o.im + im * o.re;
        re = new_re;
        im = new_im;
        return *this;
    }

    /** @brief Multiplies and assigns a scalar. */
    constexpr Complex& operator*=(double s) noexcept {
        re *= s;
        im *= s;
        return *this;
    }

    /** @brief Divides and assigns a scalar. */
    constexpr Complex& operator/=(double s) noexcept {
        re /= s;
        im /= s;
        return *this;
    }

    // ---------------------------------------------------------------------
    // Mathematical Utilities
    // ---------------------------------------------------------------------

    /**
     * @brief Computes the squared norm (magnitude squared): |z|^2 = re^2 + im^2.
     * @note Faster than mod() as it avoids a sqrt call.
     */
    constexpr double norm_sq() const noexcept {
        return re * re + im * im;
    }

    /** @brief Returns the complex conjugate z* = re - i * im. */
    constexpr Complex conjugate() const noexcept {
        return {re, -im};
    }

    /** @brief Computes the magnitude (modulus) |z| = sqrt(re^2 + im^2). */
    constexpr double mod() const noexcept;

    // ---------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------

    /** @brief Returns the real component. */
    constexpr double real() const noexcept { return re; }

    /** @brief Returns the imaginary component. */
    constexpr double imag() const noexcept { return im; }

    // ---------------------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------------------

    /** @brief Exact equality comparison. */
    constexpr bool operator==(const Complex& o) const noexcept {
        return re == o.re && im == o.im;
    }

    /** @brief Inequality comparison. */
    constexpr bool operator!=(const Complex& o) const noexcept {
        return !(*this == o);
    }
};

} // namespace qm
