#include "../include/qm.hpp"
#include <cstdio>

namespace qm {
namespace bench {

/**
 * @brief Returns the current CPU cycle counter value (TSC).
 */
inline unsigned long long cputicks() noexcept {
#if defined(__x86_64__) || defined(_M_X64)
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
#elif defined(__aarch64__)
    unsigned long long val;
    __asm__ __volatile__("mrs %0, cntvct_el0" : "=r" (val));
    return val;
#else
    return __builtin_readcyclecounter();
#endif
}

/**
 * @brief Runs an optimized stress test of the quantum compute engine.
 */
void run_core_benchmark(unsigned num_qubits, unsigned runs) {
    std::printf("==================================================\n");
    std::printf(" QUANTUM BENCHMARK FOR: %u QUBITS (Dimension: %zu)\n", 
                num_qubits, (static_cast<size_t>(1) << num_qubits));
    std::printf("==================================================\n");

    // 1. Construct a high-load test circuit
    qm::Circuit circuit(num_qubits);
    
    // Hadamard superposition layer on all qubits
    for (unsigned q = 0; q < num_qubits; ++q) {
        circuit.apply(q, qm::gates::H());
    }

    // CNOT gate cascade for canonical entanglement
    for (unsigned q = 0; q < num_qubits - 1; ++q) {
        circuit.apply_two(q, q + 1, qm::gates::CNOT());
    }

    // U3 gate layer with trigonometric evaluations
    for (unsigned q = 0; q < num_qubits; ++q) {
        circuit.apply(q, qm::gates::U3(0.5, 0.1, 0.2));
    }

    std::printf(" Circuit constructed. Total operations: %zu\n", circuit.get_instructions().size());
    std::printf(" Warming up CPU caches and allocating statevector...\n");

    // 2. SINGLE ALLOCATION (Zero-realloc in loop)
    qm::State state(num_qubits);

    // Warm up L1/L2 caches
    circuit.run_unitary(state);

    std::printf(" Executing %u benchmark iterations (zero malloc overhead)...\n", runs);

    unsigned long long total_ticks = 0;

    for (unsigned r = 0; r < runs; ++r) {
        // Reset state to |00...0> without heap reallocation
        state.reset();

        // Capture cycle count BEFORE unitary evolution
        unsigned long long start_ticks = cputicks();

        // Pure execution of the compute core
        circuit.run_unitary(state);

        // Capture cycle count AFTER
        unsigned long long end_ticks = cputicks();

        total_ticks += (end_ticks - start_ticks);
    }

    unsigned long long avg_ticks = total_ticks / runs;
    std::printf(" -> Success! Average pure core execution time: %llu CPU ticks\n\n", avg_ticks);
}

} // namespace bench
} // namespace qm

int main() {
    // Execute benchmark across target qubit grid sizes
    qm::bench::run_core_benchmark(4,  10000);
    qm::bench::run_core_benchmark(10, 1000);
    qm::bench::run_core_benchmark(14, 500);
    qm::bench::run_core_benchmark(18, 50);

    std::printf("Benchmark completed successfully.\n");
    return 0;
}
