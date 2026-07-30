#pragma once

#include "../core/types.hpp"

namespace qm {

// -------------------------------------------------------------------------
// 2x2 Matrix (Single-qubit operators)
// -------------------------------------------------------------------------
struct Matrix2x2 {
    Complex m[2][2];

    constexpr Matrix2x2() noexcept : m{{{0,0},{0,0}}, {{0,0},{0,0}}} {}
    
    constexpr Matrix2x2(Complex m00, Complex m01, Complex m10, Complex m11) noexcept
        : m{{m00, m01}, {m10, m11}} {}

    // Inline transformation for 1-qubit state vector pairs
    constexpr Complex apply_alpha(const Complex& alpha, const Complex& beta) const noexcept {
        return m[0][0] * alpha + m[0][1] * beta;
    }

    constexpr Complex apply_beta(const Complex& alpha, const Complex& beta) const noexcept {
        return m[1][0] * alpha + m[1][1] * beta;
    }

    /** @brief Returns Hermitian conjugate (dagger). */
    constexpr Matrix2x2 dagger() const noexcept {
        return Matrix2x2(
            m[0][0].conj(), m[1][0].conj(),
            m[0][1].conj(), m[1][1].conj()
        );
    }

    /** @brief Matrix-matrix multiplication. */
    constexpr Matrix2x2 operator*(const Matrix2x2& rhs) const noexcept {
        Matrix2x2 res;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                res.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j];
            }
        }
        return res;
    }
};

// -------------------------------------------------------------------------
// 4x4 Matrix (Two-qubit operators)
// -------------------------------------------------------------------------
struct Matrix4x4 {
    Complex m[4][4];

    constexpr Matrix4x4() noexcept {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = Complex{0.0, 0.0};
    }

    /** @brief In-place vector-transformation for a 4-element state slice. */
    constexpr void apply(const Complex in[4], Complex out[4]) const noexcept {
        for (int i = 0; i < 4; ++i) {
            out[i] = Complex{0.0, 0.0};
            for (int j = 0; j < 4; ++j) {
                out[i] += m[i][j] * in[j];
            }
        }
    }

    /** @brief Returns Hermitian conjugate. */
    constexpr Matrix4x4 dagger() const noexcept {
        Matrix4x4 res;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res.m[i][j] = m[j][i].conj();
            }
        }
        return res;
    }

    /**
     * @brief Tensor product (Kronecker product) of two 2x2 matrices: A ⊗ B
     */
    static constexpr Matrix4x4 kron(const Matrix2x2& a, const Matrix2x2& b) noexcept {
        Matrix4x4 res;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    for (int l = 0; l < 2; ++l) {
                        res.m[(i << 1) | k][(j << 1) | l] = a.m[i][j] * b.m[k][l];
                    }
                }
            }
        }
        return res;
    }
};

// -------------------------------------------------------------------------
// 8x8 Matrix (Three-qubit operators, e.g., Toffoli / CCX, CSWAP)
// -------------------------------------------------------------------------
struct Matrix8x8 {
    Complex m[8][8];

    constexpr Matrix8x8() noexcept {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                m[i][j] = Complex{0.0, 0.0};
    }

    constexpr void apply(const Complex in[8], Complex out[8]) const noexcept {
        for (int i = 0; i < 8; ++i) {
            out[i] = Complex{0.0, 0.0};
            for (int j = 0; j < 8; ++j) {
                out[i] += m[i][j] * in[j];
            }
        }
    }
};

} // namespace qm
