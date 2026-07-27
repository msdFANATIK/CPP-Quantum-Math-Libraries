#pragma once

namespace qm {
struct Complex {
    double re;
    double im;

    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}

    Complex operator+(const Complex& o) const {
        return {re + o.re, im + o.im};
    }

    Complex operator-(const Complex& o) const {
        return {re - o.re, im - o.im};
    }

    Complex operator*(const Complex& o) const {
        return {re * o.re - im * o.im, re * o.im + im * o.re};
    }

    double norm_sq() const {
        return re * re + im *  im;
    }

    Complex operator/(const Complex& o) const {
        double demon = o.norm_sq();
        return {
            (re * o.re + im * o.im) / demon,
            (im * o.re - re *o.im) / demon
        };
    }
};

struct Vector {
    Complex* data;
    unsigned int size;

    Vector(unsigned int s) : size(s), data(new Complex[s]{}) {}

    ~Vector() {
        delete[] data;
    }

    Complex& operator[](unsigned int i) {
        return data[i];
    }

    const Complex& operator[](unsigned int i) const {
        return data[i];
    }
};

struct Qubit {
    Complex alpha;
    Complex beta;

    Qubit(Complex a = {1.0, 0.0}, Complex b = {0.0, 0.0}) : alpha(a), beta(b) {}
};

}
