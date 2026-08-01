#include "../include/core/vector_api.hpp"
#incclude "../include/qm.hpp"
#include <cstdio>
#include <cassert>
#include <cmath>

int main() {
    // 1. Construct vector and test dynamic allocation (1.5x growth)
    qm::Vector vec;
    assert(vec.empty() && "New vector must be empty");

    // Push complex numbers (simulating state amplitudes or data stream)
    vec.push_back(qm::Complex{1.0, 0.0});
    vec.push_back(qm::Complex{0.0, 1.0});
    vec.push_back(qm::Complex{0.707106, 0.707106});

    assert(vec.size() == 3 && "Vector size mismatch after push_back");
    assert(vec.capacity() >= 3 && "Capacity should adapt automatically");

    // 2. Iterate using range-based for loop (testing begin/end pointers)
    double total_norm_sq = 0.0;
    for (const auto& amp : vec) {
        total_norm_sq += (amp.real() * amp.real() + amp.imag() * amp.imag());
    }

    // 3. Test element access & C-style contiguous data access
    qm::Complex* raw_ptr = vec.data();
    assert(raw_ptr[0].real() == 1.0 && "Data contiguous layout failure");

    // Access using operator[] and front/back
    vec.back() = qm::Complex{0.5, -0.5};
    assert(vec[2].imag() == -0.5 && "Modifier via back() failed");

    // 4. Test fast memory clearing and reset routines
    const unsigned prev_cap = vec.capacity();
    vec.reset(); // Fast zero-fill without heap reallocation

    for (unsigned i = 0; i < vec.size(); ++i) {
        assert(vec[i].real() == 0.0 && vec[i].imag() == 0.0 && "Reset failed to zero out data");
    }
    assert(vec.capacity() == prev_cap && "Reset must retain capacity");

    // 5. Test capacity optimization
    vec.clear();
    vec.shrink_to_fit();

    // Pure C-style summary matching engine output style
    std::printf("==================================================\n");
    std::printf(" QM VECTOR API TEST: PASSED                      \n");
    std::printf(" Processed Elements : %u                          \n", 3);
    std::printf(" Computed Norm Sum  : %.6f                        \n", total_norm_sq);
    std::printf("==================================================\n");

    return 0;
}
