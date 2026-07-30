#pragma once

#include "../core/types.hpp"
#include "../state/state.hpp"

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

} // namespace qm
