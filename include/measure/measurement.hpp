#pragma once

#include "../state/statevector.hpp"
#include "../math/random.hpp"

namespace qm {

/**
 * @brief Measures a single qubit and collapses the statevector.
 * @param state  State to measure (modified in-place)
 * @param qubit  Qubit index
 * @param rng    Random number generator
 * @return Measurement result (0 or 1)
 */
inline int measure_qubit(State& state, unsigned qubit, RNG& rng) noexcept {
    if (qubit >= state.n) return 0;

    const unsigned mask = 1u << qubit;
    const unsigned low_mask = mask - 1u;
    const unsigned high_mask = ~low_mask;
    const unsigned sz = state.amp.get_size();
    const unsigned half_sz = sz >> 1;

    double p0 = 0.0;
    for (unsigned i = 0; i < half_sz; ++i) {
        const unsigned k0 = ((i & high_mask) << 1) | (i & low_mask);
        p0 += state.amp[k0].norm_sq();
    }

    const double r = rng.next();
    const int result = (r < p0) ? 0 : 1;

    const double p_result = (result == 0) ? p0 : (1.0 - p0);
    if (p_result <= 0.0) return result;

    const double inv_norm = inv_sqrt(p_result);

    for (unsigned i = 0; i < half_sz; ++i) {
        const unsigned k0 = ((i & high_mask) << 1) | (i & low_mask);
        const unsigned k1 = k0 | mask;

        if (result == 0) {
            state.amp[k0] *= inv_norm;
            state.amp[k1] = {0.0, 0.0};
        } else {
            state.amp[k0] = {0.0, 0.0};
            state.amp[k1] *= inv_norm;
        }
    }

    return result;
}

/**
 * @brief Measures all qubits and collapses to a computational basis state.
 * @return Outcome as integer bitstring (0 … 2^n - 1)
 */
inline unsigned measure_all(State& state, RNG& rng) noexcept {
    const unsigned size = state.amp.get_size();
    if (size == 0) return 0;

    const double r = rng.next();
    double cumulative = 0.0;
    unsigned outcome = size - 1;

    for (unsigned i = 0; i < size; ++i) {
        cumulative += state.amp[i].norm_sq();
        if (r <= cumulative) {
            outcome = i;
            break;
        }
    }

    for (unsigned i = 0; i < size; ++i) {
        state.amp[i] = {0.0, 0.0};
    }
    state.amp[outcome] = {1.0, 0.0};

    return outcome;
}

} // namespace qm
