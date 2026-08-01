#ifndef CORE_COMPLEX_HPP
#define CORE_COMPLEX_HPP

#include <cmath>
#include <iostream>

struct Complex {
    double re;
    double im;
    constexpr Complex(double r = 0.0, double i = 0.0) noexcept : re(r), im(i) {}
    constexpr double real() const noexcept { return re; }
    constexpr double imag() const noexcept { return im; }

    constexpr Complex operator+(const Complex& o) const noexcept { return Complex(re + o.re, im + o.im); }
    constexpr Complex operator-(const Complex& o) const noexcept { return Complex(re - o.re, im - o.im); }
    constexpr Complex operator*(const Complex& o) const noexcept { return Complex(re*o.re - im*o.im, re*o.im + im*o.re); }
    constexpr Complex operator/(const Complex& o) const {
        double denom = o.re*o.re + o.im*o.im;
        return Complex((re*o.re + im*o.im)/denom, (im*o.re - re*o.im)/denom);
    }

    constexpr Complex conj() const noexcept { return Complex(re, -im); }
};

inline std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re;
    if (c.im >= 0) os << '+';
    os << c.im << 'i';
    return os;
}

#endif // CORE_COMPLEX_HPP
