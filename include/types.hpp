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

    constexpr Complex operator*(const Complex& o) const noexcept {
        return {
            re * o.re - im * o.im,
            re * o.im + im * o.re
        };
    }

    Complex operator/(const Complex& o) const noexcept {
        const double den = o.norm_sq(); 
        return {
            (re * o.re + im * o.im) / den,
            (im * o.re - re * o.im) / den
        };
    }

    Complex& operator+=(const Complex& o) noexcept {
        re += o.re;
        im += o.im;
        return *this;
    }

    Complex& operator-=(const Complex& o) noexcept {
        re -= o.re;
        im -= o.im;
        return *this;
    }

    Complex& operator*=(const Complex& o) noexcept {
        const double new_re = re * o.re - im * o.im;
        const double new_im = re * o.im + im * o.re;
        re = new_re;
        im = new_im;
        return *this;
    }

    constexpr double norm_sq() const noexcept {
        return re * re + im * im;
    }

    constexpr Complex conjugate() const noexcept {
        return {re, -im};
    }
};

struct Array {
    Complex* data;
    unsigned size;
    unsigned capacity;

    Array() noexcept : data(nullptr), size(0), capacity(0) {}

    explicit Array(unsigned n) : data(nullptr), size(n), capacity(n) {
        if (n > 0) {
            data = new Complex[n] {};
        }
    }

    Array(const Array& other) : data(nullptr), size(other.size), capacity(other.size) {
        if (size > 0) {
            data = new Complex[size];
            for (unsigned i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    Array(Array&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    Array& operator=(const Array& other) {
        if (this == &other) return *this;

        if (capacity >= other.size) {
            size = other.size;
            for (unsigned i = 0; i < size; ++i)
                data[i] = other.data[i];
            return *this;
        }

        Complex* new_data = nullptr;
        if (other.size > 0) {
            new_data = new Complex[other.size];
            for (unsigned i = 0; i < other.size; ++i)
                new_data[i] = other.data[i];
        }

        delete[] data;
        data = new_data;
        size = other.size;
        capacity = other.size;
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this == &other) return *this;

        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    ~Array() {
        delete[] data;
    }

    Complex& operator[](unsigned i) noexcept {
        return data[i];
    }

    const Complex& operator[](unsigned i) const noexcept {
        return data[i];
    }

    void resize(unsigned n) {
        if (n == size) return;

        if (n <= capacity) {
            if (n > size) {
                for (unsigned i = size; i < n; ++i)
                    data[i] = Complex{};
            }
            size = n;
            return;
        }

        Complex* new_data = new Complex[n]{};
        const unsigned copy_count = (n < size) ? n : size;
        for (unsigned i = 0; i < copy_count; ++i)
            new_data[i] = data[i];

        delete[] data;
        data = new_data;
        size = n;
        capacity = n;
    }

    void fill(const Complex& value) noexcept {
        for (unsigned i = 0; i < size; ++i)
            data[i] = value;
    }

    void clear() noexcept {
        size = 0;
    }
};

} // namespace qm
