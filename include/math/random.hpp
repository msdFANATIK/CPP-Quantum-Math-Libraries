#pragma once

#include "../core/types.hpp"
#include "../state/statevector.hpp"

namespace qm {

/**
 * @brief Permuted Congruential Generator (PCG-XSH-RR 32/64) for high-performance quantum simulations.
 * 
 * Provides fast, statistically robust pseudo-random numbers with full 53-bit mantissa precision 
 * for floating-point values and unbiased range generation.
 */
struct RNG {
    unsigned long long state; ///< Internal state of the generator.
    unsigned long long inc;   ///< Stream increment (determines the statistical sequence stream).

    /**
     * @brief Constructs and seeds the RNG.
     * 
     * @param init_state Initial state value (seed).
     * @param init_seq   Sequence selector stream identifier.
     */
    explicit RNG(unsigned long long init_state = 0x853c42e673070219ULL, unsigned long long init_seq = 0xda3e39cb9fb20720ULL) noexcept {
        seed(init_state, init_seq);
    }

    /**
     * @brief Re-seeds the generator state and stream.
     * 
     * @param init_state New state value (seed).
     * @param init_seq   New sequence selector stream identifier.
     */
    inline void seed(unsigned long long init_state, unsigned long long init_seq) noexcept {
        state = 0ULL;
        inc = (init_seq << 1u) | 1u;
        next_u32();
        state += init_state;
        next_u32();
    }

    /**
     * @brief Generates the next raw 32-bit pseudo-random integer.
     * 
     * @return A 32-bit unsigned integer.
     */
    inline unsigned int next_u32() noexcept {
        unsigned long long oldstate = state;
        state = oldstate * 6364136223846793005ULL + inc;
        unsigned int xorshifted = static_cast<unsigned int>(((oldstate >> 18u) ^ oldstate) >> 27u);
        unsigned int rot = static_cast<unsigned int>(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31u));
    }

    /**
     * @brief Generates a 64-bit pseudo-random integer by combining two 32-bit outputs.
     * 
     * @return A 64-bit unsigned integer.
     */
    inline unsigned long long next_u64() noexcept {
        return (static_cast<unsigned long long>(next_u32()) << 32) | next_u32();
    }

    /**
     * @brief Generates a double-precision floating-point number in the range [0.0, 1.0).
     * 
     * Utilizes a full 53-bit mantissa derived from `next_u64()` to ensure optimal 
     * precision for quantum probability calculations.
     * 
     * @return A double-precision random value.
     */
    inline double next() noexcept {
        return static_cast<double>(next_u64() >> 11) * 0x1.0p-53;
    }

    /**
     * @brief Generates a uniformly distributed integer within a specific range [min, max].
     * 
     * Uses Lemire's fast method to eliminate modulo bias.
     * 
     * @param min Lower bound (inclusive).
     * @param max Upper bound (inclusive).
     * @return A pseudo-random 32-bit integer within the specified range.
     */
    inline unsigned int next_range(unsigned int min, unsigned int max) noexcept {
        unsigned int range = max - min + 1;
        unsigned long long random_product = static_cast<unsigned long long>(next_u32()) * range;
        unsigned int low = static_cast<unsigned int>(random_product);
        
        if (low < range) {
            unsigned int threshold = -range % range;
            while (low < threshold) {
                random_product = static_cast<unsigned long long>(next_u32()) * range;
                low = static_cast<unsigned int>(random_product);
            }
        }
        return min + static_cast<unsigned int>(random_product >> 32);
    }

    /**
     * @brief Functor operator for quick floating-point generation.
     * 
     * Equivalent to calling `next()`.
     * 
     * @return A double-precision random value in the range [0.0, 1.0).
     */
    inline double operator()() noexcept {
        return next();
    }
};

} // namespace qm
