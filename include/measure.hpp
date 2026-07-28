#pragma once

#include "state.hpp"

namespace qm {

struct RNG {
    unsigned long state;

    explicit RNG(unsigned long seed = 123456789UL) noexcept
        : state(seed) {}

    double next() noexcept {
        // Linear Congruential Generator (Numerical Recipes)
        state = state * 1664525UL + 1013904223UL;
        return static_cast<double>(state) / 4294967296.0;   // 2^32
    }
};

inline int measure_qubit(State& state, unsigned qubit, RNG& rng) noexcept {
    if (qubit >= state.n) return 0;

    const unsigned mask = 1u << qubit;
    const unsigned size = state.amp.size;

    double p0 = 0.0;
    for (unsigned i = 0; i < size; ++i) {
        if ((i & mask) == 0) {
            p0 += state.amp[i].norm_sq();
        }
    }

    const double r = rng.next();
    const int result = (r < p0) ? 0 : 1;

    const double p_result = (result == 0) ? p0 : (1.0 - p0);
    
    if (p_result <= 0.0) {
        return result;
    }

    const double inv_norm = inv_sqrt(p_result);

    for (unsigned i = 0; i < size; ++i) {
        const bool bit = (i & mask) != 0;

        if (bit == (result == 1)) {
            state.amp[i].re *= inv_norm;
            state.amp[i].im *= inv_norm;
        } else {
            state.amp[i] = {0.0, 0.0};
        }
    }

    return result;
}

inline unsigned measure_all(State& state, RNG& rng) noexcept {
    const unsigned size = state.amp.size;

    const double r = rng.next();
    double cumulative = 0.0;
    unsigned outcome = size - 1;

    for (unsigned i = 0; i < size; ++i) {
        cumulative += state.amp[i].norm_sq();
        if (r < cumulative) {
            outcome = i;
            break;
        }
    }

    for (unsigned i = 0; i < size; ++i) {
        state.amp[i] = (i == outcome) ? Complex{1.0, 0.0} : Complex{0.0, 0.0};
    }

    return outcome;
}

inline double probability(const State& state, unsigned outcome) noexcept {
    if (outcome >= state.amp.size) return 0.0;
    return state.amp[outcome].norm_sq();
}

} // namespace qm
