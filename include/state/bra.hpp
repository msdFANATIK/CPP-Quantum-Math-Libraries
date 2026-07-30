#pragma once

#include "statevector.hpp"

namespace qm {

/**
 * @brief Bra (row vector) ⟨φ| — conjugate transpose of a Ket.
 * @details Stores the complex-conjugated amplitudes of a quantum state.
 * Allows natural quantum-mechanical Dirac notation: ⟨φ|ψ⟩ = bra * ket.
 */
struct Bra {
    Array amp;       ///< Conjugated row-vector amplitudes
    unsigned n;      ///< Number of qubits in the subsystem

    /**
     * @brief Constructs a Bra row vector from a Ket state vector.
     * @details Automatically performs complex conjugation on all amplitudes.
     * @param ket The input statevector |ψ⟩ to conjugate.
     */
    explicit Bra(const State& ket) : amp(ket.dim()), n(ket.num_qubits()) {
        const unsigned sz = ket.dim();
        for (unsigned i = 0; i < sz; ++i) {
            amp[i] = ket[i].conj();
        }
    }

    /**
     * @brief Returns the number of qubits in this state.
     * @return Unsigned number of qubits.
     */
    unsigned num_qubits() const noexcept { return n; }

    /**
     * @brief Returns the dimension of the Hilbert space.
     * @return Size of the internal amplitude array (2^n).
     */
    unsigned dim() const noexcept { return amp.get_size(); }

    /**
     * @brief Computes the inner product ⟨bra|ket⟩ (transition amplitude).
     * @details Performs a dot product between the row vector and the column vector.
     * Triggers a hardware trap if dimensions mismatch.
     * @param ket The right-hand side Ket vector |ψ⟩.
     * @return Complex scalar result of the inner product.
     */
    Complex operator*(const State& ket) const noexcept {
        const unsigned sz = amp.get_size();

        if (sz != ket.dim()) {
            __builtin_trap();
        }

        Complex sum{0.0, 0.0};
        for (unsigned i = 0; i < sz; ++i) {
            sum += amp[i] * ket[i];
        }
        return sum;
    }

    /**
     * @brief Read-only access to a specific conjugated amplitude.
     * @param i Computational basis index.
     * @return Complex amplitude at index i.
     */
    Complex operator[](unsigned i) const noexcept { return amp[i]; }

    // --- STL-compatible iterators for range-based loops ---
    const Complex* begin() const noexcept { return amp.begin(); }
    const Complex* end() const noexcept { return amp.end(); }
};

/**
 * @brief Functional helper to convert a Ket state into a Bra row vector.
 * @code
 * auto psi = qm::states::zero(1);
 * auto br = qm::bra(psi); // ⟨ψ|
 * @endcode
 * @param ket Input column state vector.
 * @return Constructed Bra object.
 */
inline Bra bra(const State& ket) {
    return Bra(ket);
}

/**
 * @brief Multiplies a Bra by a complex scalar from the left: c * ⟨ψ|.
 * @param scalar Complex scaling factor.
 * @param b Input Bra row vector.
 * @return New scaled Bra row vector.
 */
inline Bra operator*(const Complex& scalar, const Bra& b) noexcept {
    Bra res = b;
    const unsigned sz = res.dim();
    for (unsigned i = 0; i < sz; ++i) {
        res.amp[i] *= scalar;
    }
    return res;
}

/**
 * @brief Multiplies a Bra by a real scalar from the left: s * ⟨ψ|.
 * @param scalar Real scaling factor (double).
 * @param b Input Bra row vector.
 * @return New scaled Bra row vector.
 */
inline Bra operator*(double scalar, const Bra& b) noexcept {
    Bra res = b;
    const unsigned sz = res.dim();
    for (unsigned i = 0; i < sz; ++i) {
        res.amp[i] *= scalar;
    }
    return res;
}

/**
 * @brief Performs a tensor product (Kronecker product) of two Bras: ⟨ψ| ⊗ ⟨ϕ|.
 * @details Combines two row vectors into a composite joint Hilbert space row vector.
 * @param lhs Left-hand side Bra.
 * @param rhs Right-hand side Bra.
 * @return Composite Bra vector.
 */
inline Bra kron(const Bra& lhs, const Bra& rhs) {
    Bra res(State(lhs.num_qubits() + rhs.num_qubits()));
    unsigned idx = 0;
    const unsigned l_sz = lhs.dim();
    const unsigned r_sz = rhs.dim();
    for (unsigned i = 0; i < l_sz; ++i) {
        for (unsigned j = 0; j < r_sz; ++j) {
            res.amp[idx++] = lhs[i] * rhs[j];
        }
    }
    return res;
}

} // namespace qm
