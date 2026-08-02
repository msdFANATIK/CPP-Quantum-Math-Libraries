#pragma once

#include "../core/types.hpp"

namespace qm {

/**
 * @brief 2x2 Matrix for single-qubit quantum operators.
 * 
 * Represents operations acting on a single qubit, providing optimized 
 * transformations for state vector components, Hermitian conjugation, 
 * and matrix multiplication.
 */
struct Matrix2x2 {
    Complex m[2][2]; ///< Matrix elements.

    /**
     * @brief Constructs a zero-initialized 2x2 matrix.
     */
    constexpr Matrix2x2() noexcept : m{{{0,0},{0,0}}, {{0,0},{0,0}}} {}
    
    /**
     * @brief Constructs a 2x2 matrix from explicit complex components.
     */
    constexpr Matrix2x2(Complex m00, Complex m01, Complex m10, Complex m11) noexcept
        : m{{m00, m01}, {m10, m11}} {}

    /**
     * @brief Returns a 2x2 identity matrix.
     */
    static constexpr Matrix2x2 identity() noexcept {
        return Matrix2x2(
            Complex{1.0, 0.0}, Complex{0.0, 0.0},
            Complex{0.0, 0.0}, Complex{1.0, 0.0}
        );
    }

    /**
     * @brief Inline transformation for alpha component of a 1-qubit state vector pair.
     */
    constexpr Complex apply_alpha(const Complex& alpha, const Complex& beta) const noexcept {
        return m[0][0] * alpha + m[0][1] * beta;
    }

    /**
     * @brief Inline transformation for beta component of a 1-qubit state vector pair.
     */
    constexpr Complex apply_beta(const Complex& alpha, const Complex& beta) const noexcept {
        return m[1][0] * alpha + m[1][1] * beta;
    }

    /**
     * @brief Returns the Hermitian conjugate (dagger / conjugate transpose).
     */
    constexpr Matrix2x2 dagger() const noexcept {
        return Matrix2x2(
            m[0][0].conj(), m[1][0].conj(),
            m[0][1].conj(), m[1][1].conj()
        );
    }

    /**
     * @brief Performs matrix-matrix multiplication.
     */
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

/**
 * @brief 4x4 Matrix for two-qubit quantum operators.
 */
struct Matrix4x4 {
    Complex m[4][4]; ///< Matrix elements.

    /**
     * @brief Constructs a zero-initialized 4x4 matrix.
     */
    constexpr Matrix4x4() noexcept {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = Complex{0.0, 0.0};
    }

    /**
     * @brief Returns a 4x4 identity matrix.
     */
    static constexpr Matrix4x4 identity() noexcept {
        Matrix4x4 res;
        for (int i = 0; i < 4; ++i) {
            res.m[i][i] = Complex{1.0, 0.0};
        }
        return res;
    }

    /**
     * @brief Performs an in-place vector-transformation for a 4-element state slice.
     */
    constexpr void apply(const Complex in[4], Complex out[4]) const noexcept {
        for (int i = 0; i < 4; ++i) {
            out[i] = Complex{0.0, 0.0};
            for (int j = 0; j < 4; ++j) {
                out[i] += m[i][j] * in[j];
            }
        }
    }

    /**
     * @brief Returns the Hermitian conjugate.
     */
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
     * @brief Matrix-matrix multiplication for 4x4 operators.
     */
    constexpr Matrix4x4 operator*(const Matrix4x4& rhs) const noexcept {
        Matrix4x4 res;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    res.m[i][j] += m[i][k] * rhs.m[k][j];
                }
            }
        }
        return res;
    }

    /**
     * @brief Computes the tensor product (Kronecker product) of two 2x2 matrices: A ⊗ B.
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

/**
 * @brief 8x8 Matrix for three-qubit quantum operators.
 */
struct Matrix8x8 {
    Complex m[8][8]; ///< Matrix elements.

    /**
     * @brief Constructs a zero-initialized 8x8 matrix.
     */
    constexpr Matrix8x8() noexcept {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                m[i][j] = Complex{0.0, 0.0};
    }

    /**
     * @brief Returns an 8x8 identity matrix.
     */
    static constexpr Matrix8x8 identity() noexcept {
        Matrix8x8 res;
        for (int i = 0; i < 8; ++i) {
            res.m[i][i] = Complex{1.0, 0.0};
        }
        return res;
    }

    /**
     * @brief Performs an in-place vector-transformation for an 8-element state slice.
     */
    constexpr void apply(const Complex in[8], Complex out[8]) const noexcept {
        for (int i = 0; i < 8; ++i) {
            out[i] = Complex{0.0, 0.0};
            for (int j = 0; j < 8; ++j) {
                out[i] += m[i][j] * in[j];
            }
        }
    }

    /**
     * @brief Returns the Hermitian conjugate.
     */
    constexpr Matrix8x8 dagger() const noexcept {
        Matrix8x8 res;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                res.m[i][j] = m[j][i].conj();
            }
        }
        return res;
    }

    /**
     * @brief Matrix-matrix multiplication for 8x8 operators.
     */
    constexpr Matrix8x8 operator*(const Matrix8x8& rhs) const noexcept {
        Matrix8x8 res;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                for (int k = 0; k < 8; ++k) {
                    res.m[i][j] += m[i][k] * rhs.m[k][j];
                }
            }
        }
        return res;
    }

    /**
     * @brief Computes the tensor product of a 4x4 and a 2x2 matrix: A ⊗ B.
     */
    static constexpr Matrix8x8 kron(const Matrix4x4& a, const Matrix2x2& b) noexcept {
        Matrix8x8 res;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
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

} // namespace qm
