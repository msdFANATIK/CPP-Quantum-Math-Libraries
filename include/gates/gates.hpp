#pragma once

#include "../math/utils.hpp"
#include "matrix.hpp"

namespace qm {
namespace gates {

/**
 * @brief Quantum logic gates for single, two, and three-qubit systems.
 * 
 * Provides standard gate definitions as constexpr or inline matrices 
 * ready for quantum simulation backends.
 */

// -------------------------------------------------------------------------
// Single-Qubit Gates (2x2)
// -------------------------------------------------------------------------

/** @brief Identity gate (I). Leaves the qubit state unchanged. */
constexpr Matrix2x2 I() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {1.0, 0.0}
    );
}

/** @brief Pauli-X gate (NOT). Flips state components (0 to 1, 1 to 0). */
constexpr Matrix2x2 X() noexcept {
    return Matrix2x2(
        {0.0, 0.0}, {1.0, 0.0},
        {1.0, 0.0}, {0.0, 0.0}
    );
}

/** @brief Pauli-Y gate. Rotation around Y-axis by $\pi$ radians. */
constexpr Matrix2x2 Y() noexcept {
    return Matrix2x2(
        {0.0, 0.0}, {0.0, -1.0},
        {0.0, 1.0}, {0.0,  0.0}
    );
}

/** @brief Pauli-Z gate. Phase-flip gate. */
constexpr Matrix2x2 Z() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0,  0.0},
        {0.0, 0.0}, {-1.0, 0.0}
    );
}

/** @brief Hadamard gate (H). Creates an equal superposition state. */
constexpr Matrix2x2 H() noexcept {
    return Matrix2x2(
        {INV_SQRT2, 0.0}, { INV_SQRT2, 0.0},
        {INV_SQRT2, 0.0}, {-INV_SQRT2, 0.0}
    );
}

/** @brief Phase gate S ($\sqrt{Z}$). Adds a $\pi/2$ phase shift. */
constexpr Matrix2x2 S() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {0.0, 1.0}
    );
}

/** @brief S-dagger gate ($S^\dagger$). Inverse of the S gate. */
constexpr Matrix2x2 Sdag() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0,  0.0},
        {0.0, 0.0}, {0.0, -1.0}
    );
}

/** @brief T gate ($\sqrt{S}$). Adds a $\pi/4$ phase shift. */
constexpr Matrix2x2 T() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {INV_SQRT2, INV_SQRT2}
    );
}

/** @brief T-dagger gate ($T^\dagger$). Inverse of the T gate. */
constexpr Matrix2x2 Tdag() noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {INV_SQRT2, -INV_SQRT2}
    );
}

/** @brief Square root of NOT gate ($\sqrt{X}$ / SX). */
constexpr Matrix2x2 SqrtX() noexcept {
    return Matrix2x2(
        {0.5,  0.5}, {0.5, -0.5},
        {0.5, -0.5}, {0.5,  0.5}
    );
}

/**
 * @brief Arbitrary Phase Shift gate $P(\phi)$.
 * 
 * @param phi Phase angle in radians.
 */
inline Matrix2x2 Phase(double phi) noexcept {
    return Matrix2x2(
        {1.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, exp_i(phi)
    );
}

/**
 * @brief Rotation around the X-axis: $R_x(\theta) = \exp(-i \frac{\theta}{2} X)$.
 * 
 * @param theta Rotation angle in radians.
 */
inline Matrix2x2 Rx(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex c = {cos(half), 0.0};
    const Complex s = {0.0, -sin(half)};

    return Matrix2x2(c, s, s, c);
}

/**
 * @brief Rotation around the Y-axis: $R_y(\theta) = \exp(-i \frac{\theta}{2} Y)$.
 * 
 * @param theta Rotation angle in radians.
 */
inline Matrix2x2 Ry(double theta) noexcept {
    const double half = theta * 0.5;
    const double c = cos(half);
    const double s = sin(half);

    return Matrix2x2(
        {c, 0.0}, {-s, 0.0},
        {s, 0.0}, { c, 0.0}
    );
}

/**
 * @brief Rotation around the Z-axis: $R_z(\theta) = \exp(-i \frac{\theta}{2} Z)$.
 * 
 * @param theta Rotation angle in radians.
 */
inline Matrix2x2 Rz(double theta) noexcept {
    const double half = theta * 0.5;
    const Complex e_neg = exp_i(-half);
    const Complex e_pos = exp_i( half);

    return Matrix2x2(
        e_neg, {0.0, 0.0},
        {0.0, 0.0}, e_pos
    );
}

/**
 * @brief Universal single-qubit gate $U_3(\theta, \phi, \lambda)$.
 * 
 * @param theta Polar angle.
 * @param phi   Azimuthal angle.
 * @param lambda Phase angle.
 */
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

/** @brief Controlled-NOT (CNOT / CX) gate. */
constexpr Matrix4x4 CNOT() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][3] = {1.0, 0.0};
    res.m[3][2] = {1.0, 0.0};
    return res;
}

/** @brief Controlled-Y (CY) gate. */
constexpr Matrix4x4 CY() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][3] = {0.0, -1.0};
    res.m[3][2] = {0.0,  1.0};
    return res;
}

/** @brief Controlled-Z (CZ) gate. */
constexpr Matrix4x4 CZ() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][2] = {1.0, 0.0};
    res.m[3][3] = {-1.0, 0.0};
    return res;
}

/** @brief Controlled-Hadamard (CH) gate. */
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

/**
 * @brief Controlled-Phase gate ($CPHASE(\phi)$ / $CR_z(\phi)$).
 * 
 * @param phi Phase angle in radians.
 */
inline Matrix4x4 CPhase(double phi) noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][1] = {1.0, 0.0};
    res.m[2][2] = {1.0, 0.0};
    res.m[3][3] = exp_i(phi);
    return res;
}

/**
 * @brief Controlled-Rx rotation gate ($CR_x(\theta)$).
 * 
 * @param theta Rotation angle in radians.
 */
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

/** @brief SWAP gate. Swaps states of two qubits. */
constexpr Matrix4x4 SWAP() noexcept {
    Matrix4x4 res{};
    res.m[0][0] = {1.0, 0.0};
    res.m[1][2] = {1.0, 0.0};
    res.m[2][1] = {1.0, 0.0};
    res.m[3][3] = {1.0, 0.0};
    return res;
}

/** @brief Square root of SWAP ($\sqrt{\text{SWAP}}$) gate. */
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

/** @brief iSWAP gate. */
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

/** @brief Toffoli (CCNOT) gate. Control qubits 0 and 1, target qubit 2. */
constexpr Matrix8x8 TOFFOLI() noexcept {
    Matrix8x8 res{};
    for (int i = 0; i < 6; ++i) {
        res.m[i][i] = {1.0, 0.0};
    }
    res.m[6][7] = {1.0, 0.0};
    res.m[7][6] = {1.0, 0.0};
    return res;
}

/** @brief CSWAP (Fredkin) gate. Control qubit 0, target qubits 1 and 2. */
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
