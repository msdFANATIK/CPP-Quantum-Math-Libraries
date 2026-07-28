#pragma once

#include "state.hpp"

namespace qm {

using uint32_t = unsigned int;
using uint64_t = unsigned long long;

struct RNG {
    uint64_t state;
    uint64_t inc;

    explicit RNG(uint64_t init_state = 0x853c42e673070219ULL, uint64_t init_seq = 0xda3e39cb9fb20720ULL) noexcept
        : state(0ULL), inc((init_seq << 1u) | 1u) {
        next_u32();
        state += init_state;
        next_u32();
    }

    inline uint32_t next_u32() noexcept {
        uint64_t oldstate = state;

        state = oldstate * 6364136223846793005ULL + inc;

        uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31u));
    }

    // [0.0, 1.0)
    inline double next() noexcept {
        constexpr double INV_2_32 = 2.3283064365386962890625e-10; // 1.0 / 2^32
        return static_cast<double>(next_u32()) * INV_2_32;
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
