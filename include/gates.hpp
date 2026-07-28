#pragma once

#include "utils.hpp"

namespace qm {

struct Matrix2x2 {
    Complex m[2][2];

    constexpr Matrix2x2() noexcept : m{{{0,0},{0,0}}, {{0,0},{0,0}}} {}
    constexpr Matrix2x2(Complex m00, Complex m01, Complex m10, Complex m11) noexcept
        : m{{m00, m01}, {m10, m11}} {}

    constexpr Complex apply_alpha(const Complex& alpha, const Complex& beta) const noexcept {
        return m[0][0] * alpha + m[0][1] * beta;
    }

    constexpr Complex apply_beta(const Complex& alpha, const Complex& beta) const noexcept {
        return m[1][0] * alpha + m[1][1] * beta;
    }
};

struct Matrix4x4 {
    Complex m[4][4];

    constexpr Matrix4x4() noexcept {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = Complex{0.0, 0.0};
    }
};

namespace gates {

// Identity (I)
constexpr Matrix2x2 I() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {1.0, 0.0}
    );
}

// Pauli-X (NOT)
constexpr Matrix2x2 X() noexcept {
    return Matrix2x2(
        {0.0, 0.0}, {1.0, 0.0},
        {1.0, 0.0}, {0.0, 0.0}
    );
}

// Pauli-Y
constexpr Matrix2x2 Y() noexcept {
    return Matrix2x2(
        {0.0, 0.0}, {0.0, -1.0},
        {0.0, 1.0}, {0.0,  0.0}
    );
}

// Pauli-Z
constexpr Matrix2x2 Z() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0,  0.0},
        {0.0, 0.0}, {-1.0, 0.0}
    );
}

// Hadamard (H)
constexpr Matrix2x2 H() noexcept {
    return Matrix2x2(
        {INV_SQRT2, 0.0}, { INV_SQRT2, 0.0},
        {INV_SQRT2, 0.0}, {-INV_SQRT2, 0.0}
    );
}

// Phase gate S = √Z
constexpr Matrix2x2 S() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {0.0, 1.0}
    );
}

// T gate = √S
constexpr Matrix2x2 T() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {INV_SQRT2, INV_SQRT2}
    );
}

// Rx(θ)
inline Matrix2x2 Rx(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex c = {cos(half), 0.0};
    const Complex s = {0.0, -sin(half)};

    return Matrix2x2(c, s, s, c);
}

// Ry(θ)
inline Matrix2x2 Ry(double theta) noexcept {
    const double half = theta * 0.5;
    const double c = cos(half);
    const double s = sin(half);

    return Matrix2x2(
        {c, 0.0}, {-s, 0.0},
        {s, 0.0}, { c, 0.0}
    );
}

// Rz(θ)
inline Matrix2x2 Rz(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex e_neg = exp_i(-half);
    const Complex e_pos = exp_i( half);

    return Matrix2x2(
        e_neg, {0.0, 0.0},
        {0.0, 0.0}, e_pos
    );
}

// Controlled-NOT (CNOT) — control=0, target=1
constexpr Matrix4x4 CNOT() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][3] = {1.0, 0.0};
    res.m[3][2] = {1.0, 0.0};
    return res;
}

// Controlled-Z (CZ)
constexpr Matrix4x4 CZ() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][2] = {1.0, 0.0};
    res.m[3][3] = {-1.0, 0.0};
    return res;
}

// SWAP
constexpr Matrix4x4 SWAP() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][2] = {1.0, 0.0};
    res.m[2][1] = {1.0, 0.0};
    res.m[3][3] = {1.0, 0.0};
    return res;
}

} // namespace gates

} // namespace qm
