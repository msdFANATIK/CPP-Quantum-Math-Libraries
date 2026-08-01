#include "qm.hpp"
#include <cstdio>
#include <cmath>
#include <cassert>

int main() {
    constexpr unsigned num_qubits = 2;

    // 1. Allocate zero-initialized state vector |00>
    qm::State state(num_qubits);

    // 2. Build Bell-state circuit: (|00> + |11>) / sqrt(2)
    qm::Circuit circuit(num_qubits);
    circuit.apply(0, qm::gates::H());
    circuit.apply_two(0, 1, qm::gates::CNOT());

    // 3. Execute quantum compute pipeline
    circuit.run_unitary(state);

    // 4. Validate statevector norm (Dimension is 2^N)
    const size_t dim = static_cast<size_t>(1) << num_qubits;
    double norm_sq = 0.0;

    for (size_t i = 0; i < dim; ++i) {
        qm::Scalar amp = state[i];
        // Using function calls real() and imag() as reported by compiler
        norm_sq += (amp.real() * amp.real() + amp.imag() * amp.imag());
    }

    // High-precision assertion to trap execution bugs in CI
    assert(std::abs(norm_sq - 1.0) < 1e-9 && "Statevector norm violated!");

    // Pure C-style summary matching engine benchmark output
    std::printf("==================================================\n");
    std::printf(" QM ENGINE SMOKE TEST: PASSED                      \n");
    std::printf(" Target Qubits : %u (Dimension: %zu)              \n", num_qubits, dim);
    std::printf(" State Norm    : %.9f                            \n", std::sqrt(norm_sq));
    std::printf("==================================================\n");

    return 0;
}
