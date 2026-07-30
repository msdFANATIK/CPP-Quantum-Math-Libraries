#pragma once

#include "statevector.hpp"

namespace qm {

/**
 * @brief Ket (column statevector) |ψ⟩ in Dirac notation.
 * @details Represents the state of a quantum system as a column vector
 * in the Hilbert space. This is a semantic alias for State.
 */
using Ket = State;

namespace states {

/**
 * @brief Creates a computational basis state |00...0⟩ for a given number of qubits.
 * @details Creates the pure state |0...0⟩. The State constructor already allocates
 * 2^n elements, zeroes them, and sets amplitude 1.0 at index 0.
 * @param num_qubits Number of qubits in the register.
 * @return Ket |0⟩_n state vector.
 */
inline Ket zero(unsigned num_qubits) {
    return Ket(num_qubits);
}

/**
 * @brief Creates a single-qubit computational basis state |1⟩.
 * @details Hilbert space dimension is 2. Amplitude of |0⟩ is set to 0,
 * amplitude of |1⟩ is set to 1.
 * @return Ket |1⟩ state vector.
 */
inline Ket one() {
    Ket psi(1);
    psi[0] = {0.0, 0.0};
    psi[1] = {1.0, 0.0};
    return psi;
}

/**
 * @brief Creates a single-qubit symmetric superposition state |+⟩.
 * @details |+⟩ = (|0⟩ + |1⟩) / √2. Represents the uniform superposition.
 * @return Ket |+⟩ state vector.
 */
inline Ket plus() {
    Ket psi(1);
    psi[0] = {INV_SQRT2, 0.0};
    psi[1] = {INV_SQRT2, 0.0};
    return psi;
}

/**
 * @brief Creates a single-qubit asymmetric superposition state |-⟩.
 * @details |-⟩ = (|0⟩ - |1⟩) / √2. Phase-flipped superposition.
 * @return Ket |-⟩ state vector.
 */
inline Ket minus() {
    Ket psi(1);
    psi[0] = {INV_SQRT2, 0.0};
    psi[1] = {-INV_SQRT2, 0.0};
    return psi;
}

} // namespace states

/**
 * @brief Calculates the Born probability of measuring a specific basis state index.
 * @details P(i) = |α_i|² (square of the amplitude magnitude).
 * Uses fast norm_sq() to avoid costly sqrt calculations.
 * @param ket The quantum state vector |ψ⟩.
 * @param state_index Computational basis index (0 to 2^n - 1).
 * @return Double precision probability value in range [0.0, 1.0].
 */
inline double probability(const Ket& ket, unsigned state_index) noexcept {
    if (state_index >= ket.dim()) {
        __builtin_trap();
    }
    return ket[state_index].norm_sq();
}

/**
 * @brief Computes the total probability (norm squared) of the quantum statevector.
 * @details For a valid normalized physical state this must equal exactly 1.0.
 * Useful for debugging and tracking numerical drift during simulation.
 * @param ket The quantum state vector |ψ⟩.
 * @return Sum of all squared amplitudes.
 */
inline double total_probability(const Ket& ket) noexcept {
    double total = 0.0;
    const unsigned sz = ket.dim();
    for (unsigned i = 0; i < sz; ++i) {
        total += ket[i].norm_sq();
    }
    return total;
}

/**
 * @brief Multiplies a Ket by a complex scalar from the left: c * |ψ⟩.
 * @param scalar Complex scaling factor.
 * @param ket Input state vector.
 * @return New scaled Ket vector.
 */
inline Ket operator*(const Complex& scalar, const Ket& ket) noexcept {
    Ket res = ket;
    const unsigned sz = res.dim();
    for (unsigned i = 0; i < sz; ++i) {
        res[i] *= scalar;
    }
    return res;
}

/**
 * @brief Multiplies a Ket by a real scalar from the left: s * |ψ⟩.
 * @param scalar Real scaling factor (double).
 * @param ket Input state vector.
 * @return New scaled Ket vector.
 */
inline Ket operator*(double scalar, const Ket& ket) noexcept {
    Ket res = ket;
    const unsigned sz = res.dim();
    for (unsigned i = 0; i < sz; ++i) {
        res[i] *= scalar;
    }
    return res;
}

/**
 * @brief Performs a tensor product (Kronecker product) of two Kets: |ψ⟩ ⊗ |ϕ⟩.
 * @details Combines two independent quantum subsystems into a single composite system.
 * Total qubits = ket1.qubits + ket2.qubits. Total dimension = dim1 * dim2.
 * @param lhs Left-hand side Ket.
 * @param rhs Right-hand side Ket.
 * @return Composite Ket vector in the joint Hilbert space.
 */
inline Ket kron(const Ket& lhs, const Ket& rhs) {
    Ket res(lhs.num_qubits() + rhs.num_qubits());

    unsigned idx = 0;
    const unsigned l_sz = lhs.dim();
    const unsigned r_sz = rhs.dim();

    for (unsigned i = 0; i < l_sz; ++i) {
        for (unsigned j = 0; j < r_sz; ++j) {
            res[idx++] = lhs[i] * rhs[j];
        }
    }
    return res;
}

} // namespace qm
