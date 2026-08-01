#include "../include/qm.hpp"
#include <cstdio>
#include <cmath>
#include <cassert>

int main() {
    constexpr unsigned num_qubits = 2;

    // 1. Allocate zero-initialized state vector |00>
    qm::State state(num_qubits);

    // 2. Build Bell-state circuit: (|00> + |11>) / sqrt(2)
    qm::Circuit circuit(num_qubits);
    circuit.apply(0, qm::gates::H());              // Superposition on Qubit 0
    circuit.apply_two(0, 1, qm::gates::CNOT());     // Entangle Qubit 0 -> Qubit 1

    // 3. Execute quantum compute pipeline
    circuit.run_unitary(state);

    // 4. Validate statevector norm (Probability conservation: sum(|psi|^2) == 1.0)
    double norm_sq = 0.0;
    for (size_t i = 0; i < state.size(); ++i) {
        qm::Scalar amp = state[i];
        norm_sq += (amp.real * amp.real + amp.imag * amp.imag);
    }

    // High-precision assertion to trap execution bugs in CI
    assert(std::abs(norm_sq - 1.0) < 1e-9 && "Statevector norm violated!");

    // Pure C-style summary matching engine benchmark output
    std::printf("==================================================\n");
    std::printf(" QM ENGINE SMOKE TEST: PASSED                      \n");
    std::printf(" Target Qubits : %u (Dimension: %zu)              \n", num_qubits, state.size());
    std::printf(" State Norm    : %.9f                            \n", std::sqrt(norm_sq));
    std::printf("==================================================\n");

    return 0;
}
