#pragma once

#include "../math/utils.hpp"
#include "matrix.hpp"

namespace qm {
namespace gates {

// -------------------------------------------------------------------------
// Single-Qubit Gates (2x2)
// -------------------------------------------------------------------------

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

// Phase gate S = √Z = diag(1, i)
constexpr Matrix2x2 S() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {0.0, 1.0}
    );
}

// S† (S-dagger)
constexpr Matrix2x2 Sdag() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0,  0.0},
        {0.0, 0.0}, {0.0, -1.0}
    );
}

// T gate = √S = diag(1, e^(iπ/4))
constexpr Matrix2x2 T() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {INV_SQRT2, INV_SQRT2}
    );
}

// T† (T-dagger)
constexpr Matrix2x2 Tdag() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {INV_SQRT2, -INV_SQRT2}
    );
}

// Square root of NOT (√X / SX)
constexpr Matrix2x2 SqrtX() noexcept {
    return Matrix2x2(
        {0.5,  0.5}, {0.5, -0.5},
        {0.5, -0.5}, {0.5,  0.5}
    );
}

// Arbitrary Phase Shift P(phi) = diag(1, e^(i*phi))
inline Matrix2x2 Phase(double phi) noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, exp_i(phi)
    );
}

// Rotation Rx(θ) = exp(-i * θ/2 * X)
inline Matrix2x2 Rx(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex c = {cos(half), 0.0};
    const Complex s = {0.0, -sin(half)};

    return Matrix2x2(c, s, s, c);
}

// Rotation Ry(θ) = exp(-i * θ/2 * Y)
inline Matrix2x2 Ry(double theta) noexcept {
    const double half = theta * 0.5;
    const double c = cos(half);
    const double s = sin(half);

    return Matrix2x2(
        {c, 0.0}, {-s, 0.0},
        {s, 0.0}, { c, 0.0}
    );
}

// Rotation Rz(θ) = exp(-i * θ/2 * Z)
inline Matrix2x2 Rz(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex e_neg = exp_i(-half);
    const Complex e_pos = exp_i( half);

    return Matrix2x2(
        e_neg, {0.0, 0.0},
        {0.0, 0.0}, e_pos
    );
}

// Universal Single-Qubit Gate U3(theta, phi, lambda)
inline Matrix2x2 U3(double theta, double phi, double lambda) noexcept {
    const double half = theta * 0.5;
    const double c = cos(half);
    const double s = sin(half);

    return Matrix2x2(
        {c, 0.0},                     -exp_i(lambda) * s,
        exp_i(phi) * s,               exp_i(phi + lambda) * c
    );
}

// -------------------------------------------------------------------------
// Two-Qubit Gates (4x4)
// -------------------------------------------------------------------------

// Controlled-NOT (CNOT / CX)
constexpr Matrix4x4 CNOT() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][3] = {1.0, 0.0};
    res.m[3][2] = {1.0, 0.0};
    return res;
}

// Controlled-Y (CY)
constexpr Matrix4x4 CY() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][3] = {0.0, -1.0};
    res.m[3][2] = {0.0,  1.0};
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

// Controlled-Hadamard (CH)
constexpr Matrix4x4 CH() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][2] = {INV_SQRT2, 0.0};
    res.m[2][3] = {INV_SQRT2, 0.0};
    res.m[3][2] = {INV_SQRT2, 0.0};
    res.m[3][3] = {-INV_SQRT2, 0.0};
    return res;
}

// Controlled-Phase CPHASE(phi) / CRz(phi)
inline Matrix4x4 CPhase(double phi) noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][2] = {1.0, 0.0};
    res.m[3][3] = exp_i(phi);
    return res;
}

// Controlled-Rx CRx(theta)
inline Matrix4x4 CRx(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex c = {cos(half), 0.0};
    const Complex s = {0.0, -sin(half)};

    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][2] = c;
    res.m[2][3] = s;
    res.m[3][2] = s;
    res.m[3][3] = c;
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

// √SWAP (Square Root of SWAP)
constexpr Matrix4x4 SqrtSWAP() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {0.5, 0.5};
    res.m[1][2] = {0.5, -0.5};
    res.m[2][1] = {0.5, -0.5};
    res.m[2][2] = {0.5, 0.5};
    res.m[3][3] = {1.0, 0.0};
    return res;
}

// iSWAP
constexpr Matrix4x4 iSWAP() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][2] = {0.0, 1.0};
    res.m[2][1] = {0.0, 1.0};
    res.m[3][3] = {1.0, 0.0};
    return res;
}

// -------------------------------------------------------------------------
// Three-Qubit Gates (8x8)
// -------------------------------------------------------------------------

// Toffoli (CCNOT) — control1=0, control2=1, target=2
constexpr Matrix8x8 TOFFOLI() noexcept {
    Matrix8x8 res{};
    for (int i = 0; i < 6; ++i) {
        res.m[i][i] = {1.0, 0.0};
    }
    res.m[6][7] = {1.0, 0.0};
    res.m[7][6] = {1.0, 0.0};
    return res;
}

// CSWAP (Fredkin) — control=0, target1=1, target2=2
constexpr Matrix8x8 CSWAP() noexcept {
    Matrix8x8 res{};
    for (int i = 0; i < 5; ++i) {
        res.m[i][i] = {1.0, 0.0};
    }
    res.m[5][6] = {1.0, 0.0};
    res.m[6][5] = {1.0, 0.0};
    res.m[7][7] = {1.0, 0.0};
    return res;
}

} // namespace gates
} // namespace qm
