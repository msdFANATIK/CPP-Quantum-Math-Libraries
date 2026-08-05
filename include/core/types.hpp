#ifndef QM_COMPLEX_HPP
#define QM_COMPLEX_HPP

namespace qm {

/**
 * @file types.hpp
 * @brief High-performance zero-dependency complex number structure for quantum state computation.
 */

/**
 * @brief Lightweight representation of a complex number z = re + i * im.
 * 
 * Fully standalone implementation optimized for SIMD/FMA instruction auto-vectorization.
 * Operates with absolute zero dependencies on C/C++ standard math libraries.
 */
struct Complex {
    double re; ///< Real component of the complex number.
    double im; ///< Imaginary component of the complex number.

    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    /**
     * @brief Constructs a complex number from real and imaginary components.
     * @param r Real part (defaults to 0.0).
     * @param i Imaginary part (defaults to 0.0).
     */
    constexpr Complex(double r = 0.0, double i = 0.0) noexcept : re(r), im(i) {}

    // ---------------------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------------------

    /** @brief Returns negation of the complex number (-z). */
    constexpr Complex operator-() const noexcept {
        return {-re, -im};
    }

    /** @brief Unary plus operator (+z). */
    constexpr Complex operator+() const noexcept {
        return *this;
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
        const double inv_den = (den != 0.0) ? (1.0 / den) : 0.0;
        return {
            (re * o.re + im * o.im) * inv_den,
            (im * o.re - re * o.im) * inv_den
        };
    }

    // ---------------------------------------------------------------------
    // Binary Arithmetic Operators (Scalar Operations)
    // ---------------------------------------------------------------------

    constexpr Complex operator+(double s) const noexcept { return {re + s, im}; }
    constexpr Complex operator-(double s) const noexcept { return {re - s, im}; }
    constexpr Complex operator*(double s) const noexcept { return {re * s, im * s}; }
    constexpr Complex operator/(double s) const noexcept { 
        const double inv_s = 1.0 / s;
        return {re * inv_s, im * inv_s}; 
    }

    friend constexpr Complex operator+(double s, const Complex& z) noexcept { return {s + z.re, z.im}; }
    friend constexpr Complex operator-(double s, const Complex& z) noexcept { return {s - z.re, -z.im}; }
    friend constexpr Complex operator*(double s, const Complex& z) noexcept { return z * s; }
    friend constexpr Complex operator/(double s, const Complex& z) noexcept {
        const double den = z.norm_sq();
        const double inv_den = (den != 0.0) ? (s / den) : 0.0;
        return {inv_den * z.re, -inv_den * z.im};
    }

    // ---------------------------------------------------------------------
    // Compound Assignment Operators
    // ---------------------------------------------------------------------

    constexpr Complex& operator+=(const Complex& o) noexcept {
        re += o.re;
        im += o.im;
        return *this;
    }

    constexpr Complex& operator-=(const Complex& o) noexcept {
        re -= o.re;
        im -= o.im;
        return *this;
    }

    constexpr Complex& operator*=(const Complex& o) noexcept {
        const double n_re = re * o.re - im * o.im;
        const double n_im = re * o.im + im * o.re;
        re = n_re;
        im = n_im;
        return *this;
    }

    constexpr Complex& operator/=(const Complex& o) noexcept {
        *this = *this / o;
        return *this;
    }

    constexpr Complex& operator+=(double s) noexcept { re += s; return *this; }
    constexpr Complex& operator-=(double s) noexcept { re -= s; return *this; }
    constexpr Complex& operator*=(double s) noexcept { re *= s; im *= s; return *this; }
    constexpr Complex& operator/=(double s) noexcept { 
        const double inv_s = 1.0 / s;
        re *= inv_s; 
        im *= inv_s; 
        return *this; 
    }

    // ---------------------------------------------------------------------
    // Mathematical Utilities & Fast Hardware Intrinsics
    // ---------------------------------------------------------------------

    /**
     * @brief Computes squared magnitude |z|^2 = re^2 + im^2.
     * @note Avoids square root calculation for fast comparisons.
     */
    constexpr double norm_sq() const noexcept {
        return re * re + im * im;
    }

    /** @brief Returns complex conjugate z* = re - i * im. */
    constexpr Complex conj() const noexcept {
        return {re, -im};
    }

    /**
     * @brief Computes complex modulus |z| = sqrt(re^2 + im^2) using direct hardware FPU/AVX instruction.
     */
    double mod() const noexcept {
        const double val = norm_sq();
        if (val <= 0.0) return 0.0;

        double res = val;
#if defined(__x86_64__) || defined(_M_X64)
        asm volatile("sqrtsd %1, %0" : "=x"(res) : "x"(val));
#elif defined(__aarch64__)
        asm volatile("fsqrt %d0, %d1" : "=w"(res) : "w"(val));
#else
        // Zero-stdlib Newton-Raphson fallback algorithm
        double x = val * 0.5;
        for (int i = 0; i < 6; ++i) {
            res = 0.5 * (res + val / res);
        }
#endif
        return res;
    }

    /**
     * @brief Creates complex number from polar coordinates e^(i * phi).
     * Computes unit magnitude state phase shift.
     */
    static Complex from_polar(double r, double theta) noexcept {
        // Polynomial zero-stdlib Taylor/CORDIC approximation for cos/sin unit phase
        // Useful for quantum gates computation without <cmath>
        auto fast_cos = [](double x) noexcept -> double {
            // Map angle to [-PI, PI]
            while (x > 3.14159265358979323846) x -= 6.28318530717958647692;
            while (x < -3.14159265358979323846) x += 6.28318530717958647692;
            const double x2 = x * x;
            return 1.0 - x2 * (0.5 - x2 * (1.0 / 24.0 - x2 * (1.0 / 720.0)));
        };

        auto fast_sin = [](double x) noexcept -> double {
            while (x > 3.14159265358979323846) x -= 6.28318530717958647692;
            while (x < -3.14159265358979323846) x += 6.28318530717958647692;
            const double x2 = x * x;
            return x * (1.0 - x2 * (1.0 / 6.0 - x2 * (1.0 / 120.0 - x2 * (1.0 / 5040.0))));
        };

        return {r * fast_cos(theta), r * fast_sin(theta)};
    }

    // ---------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------

    constexpr double real() const noexcept { return re; }
    constexpr double imag() const noexcept { return im; }

    // ---------------------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------------------

    constexpr bool operator==(const Complex& o) const noexcept {
        return re == o.re && im == o.im;
    }

    constexpr bool operator!=(const Complex& o) const noexcept {
        return !(*this == o);
    }
};

} // namespace qm

#endif // QM_COMPLEX_HPP
