#pragma once

#include "types.hpp"

namespace qm {

struct RNG {
    unsigned long state;

    RNG(unsigned long seed = 123456789) : state(seed) {}

    double next_double() {
        state = (state * 1664525UL + 1013904223UL) & 0xFFFFFFFFUL;
        return static_cast<double>(state) / 4294967296.0;
    }
};

inline int measure(Qubit& q, RNG& rng) {
    double p0 = q.alpha.norm_sq();
    double random_val = rng.next_double();

    if (random_val < p0) {
        q.alpha = {1.0, 0.0};
        q.beta = {0.0, 0.0};
        return 0;
    } else {
        q.alpha = {0.0, 0.0};
        q.beta = {1.0, 0.0};
        return 1;
    }
}
}
