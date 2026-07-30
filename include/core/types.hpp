#pragma once


namespace qm {

struct Complex {
    double re;
    double im;

    constexpr Complex(double r = 0, double i = 0) noexcept : re(r), im(i) {}

    constexpr Complex operator+(const Complex& o) const noexcept {
        return {re + o.re, im + o.im};
    }

    constexpr Complex operator-(const Complex& o) const noexcept {
        return {re - o.re, im - o.im};
    }

    constexpr Complex operator-() const noexcept {
        return {-re, -im};
    }

    constexpr Complex operator*(const Complex& o) const noexcept {
        return {
            re * o.re - im * o.im,
            re * o.im + im * o.re
        };
    }

    friend constexpr Complex operator*(double s, const Complex& z) noexcept {
        return z * s;
    } 

    constexpr Complex operator/(const Complex& o) const noexcept {
        const double den = o.norm_sq(); 
        return {
            (re * o.re + im * o.im) / den,
            (im * o.re - re * o.im) / den
        };
    }

    constexpr Complex operator/(double s) const noexcept {
        return {re / s, im / s};
    }

    friend Complex operator/(double s, const Complex& z) noexcept {
        const double den = z.norm_sq();
        return {s * z.re / den, -s * z.im / den};
    }

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
        const double new_re = re * o.re - im * o.im;
        const double new_im = re * o.im + im * o.re;
        re = new_re;
        im = new_im;
        return *this;
    }

    constexpr Complex& operator*=(double s) noexcept {
        re *= s;
        im *= s;
        return *this;
    } 
    
    constexpr Complex& operator/=(double s) noexcept {
        re /= s;
        im /= s;
        return *this;
    }

    constexpr double norm_sq() const noexcept {
        return re * re + im * im;
    }

    constexpr Complex conjugate() const noexcept {
        return {re, -im};
    }

    constexpr bool operator==(const Complex& o) const noexcept {
        return re == o.re && im == o.im;
    }

    constexpr bool operator!=(const Complex& o) const noexcept {
       return !(*this == o);
    }

    constexpr double real() const noexcept { return re; }
    constexpr double imag() const noexcept { return im; }

    constexpr double mod() const noexcept;
};


} // namespace qm
