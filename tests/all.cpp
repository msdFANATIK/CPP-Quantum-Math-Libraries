#include "qm.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>

using namespace qm;
using Clock = std::chrono::high_resolution_clock;

#define CHECK(cond) \
    if (!(cond)) { \
        std::cout << " [FAIL] " << #cond << " (Line " << __LINE__ << ")\n"; \
        return 1; \
    }

#define SECTION(name) std::cout << "\n=== " << name << " ===\n";

int main() {
    std::cout << "Quantum++ Mini Engine — Core Integration Test Suite\n";
    std::cout << "====================================================\n";

    // -------------------------------------------------------
    // 1. Complex Algebra & Hilbert State Properties
    // -------------------------------------------------------
    SECTION("1. Complex Algebra & Hilbert State Validation")
    {
       // Vector norm and squared magnitude
        Complex a{3.0, 4.0};
        CHECK(a.norm_sq() == 25.0);

        // Imaginary unit multiplication (i * i = -1)
        Complex i_unit{0.0, 1.0};
        Complex i_sq = i_unit * i_unit;
        CHECK(std::abs(i_sq.re + 1.0) < 1e-12);
        CHECK(std::abs(i_sq.im) < 1e-12);

        // exp_i accuracy checks
        Complex e0 = exp_i(0.0);
        CHECK(std::abs(e0.re - 1.0) < 1e-10);
        CHECK(std::abs(e0.im) < 1e-10);

        Complex e_half_pi = exp_i(PI * 0.5);
        CHECK(std::abs(e_half_pi.re) < 1e-8);
        CHECK(std::abs(e_half_pi.im - 1.0) < 1e-8);

        Complex e_pi = exp_i(PI);
        CHECK(std::abs(e_pi.re + 1.0) < 1e-8);
        CHECK(std::abs(e_pi.im) < 1e-8);

        // Inner product orthogonality check for pure states
        Complex zero_state{1.0, 0.0};
        Complex one_state{0.0, 0.0};
        double norm_val = zero_state.norm_sq() + one_state.norm_sq();
        CHECK(std::abs(norm_val - 1.0) < 1e-20);

        std::cout << " Complex Algebra & Hilbert State: OK\n";
    }

    // -------------------------------------------------------
    // 2. Unitary Gate Identities
    // -------------------------------------------------------
    SECTION("2. Unitary Gate Identities (H^2 = X^2 = Y^2 = Z^2 = I)")
    {
        // Hadamard involution: H * H = I
        State s(1);
        s.apply(gates::H(), 0);
        s.apply(gates::H(), 0);
        CHECK(std::abs(s.amp[0].re - 1.0) < 1e-10);
        CHECK(s.amp[1].norm_sq() < 1e-20);

        // Pauli-X involution: X * X = I
        State sx(1);
        sx.apply(gates::X(), 0);
        sx.apply(gates::X(), 0);
        CHECK(std::abs(sx.amp[0].re - 1.0) < 1e-10);

        // Pauli-Y involution: Y * Y = I
        State sy(1);
        sy.apply(gates::Y(), 0);
        sy.apply(gates::Y(), 0);
        CHECK(std::abs(sy.amp[0].re - 1.0) < 1e-10);

        // Pauli-Z involution: Z * Z = I
        State sz(1);
        sz.apply(gates::Z(), 0);
        sz.apply(gates::Z(), 0);
        CHECK(std::abs(sz.amp[0].re - 1.0) < 1e-10);

        std::cout << " Unitary Gate Identities: OK\n";
    }

    // -------------------------------------------------------
    // 3. Maximally Entangled Bell State Validation
    // -------------------------------------------------------
    SECTION("3. Bell State Preparation (|Phi+>)")
    {
        State bell(2);
        bell.apply(gates::H(), 0);
        bell.apply_cnot(0, 1);

        // Verify state |Phi+> = (|00> + |11>) / sqrt(2)
        CHECK(std::abs(bell.amp[0].re - INV_SQRT2) < 1e-10);
        CHECK(std::abs(bell.amp[3].re - INV_SQRT2) < 1e-10);
        CHECK(bell.amp[1].norm_sq() < 1e-20);
        CHECK(bell.amp[2].norm_sq() < 1e-20);

        // Verify probability densities
        CHECK(std::abs(probability(bell, 0) - 0.5) < 1e-10);
        CHECK(std::abs(probability(bell, 3) - 0.5) < 1e-10);

        std::cout << " Bell Amplitudes: OK\n";
    }

    // -------------------------------------------------------
    // 4. Statistical Measurement Sampling
    // -------------------------------------------------------
    SECTION("4. Statistical Measurement Sampling (Monte Carlo)")
    {
        State bell(2);
        bell.apply(gates::H(), 0);
        bell.apply_cnot(0, 1);

        RNG rng(123456789UL); // Fixed seed for reproducible test run

        const int N = 50000;
        int count00 = 0, count11 = 0, invalid_states = 0;

        for (int i = 0; i < N; ++i) {
            State temp = bell;
            unsigned res = measure_all(temp, rng);
            if (res == 0) ++count00;
            else if (res == 3) ++count11;
            else ++invalid_states;
        }

        double p00 = 100.0 * count00 / N;
        double p11 = 100.0 * count11 / N;

        std::cout << " Shots evaluated: " << N << "\n";
        std::cout << " |00> outcome : " << count00 << " (" << p00 << "%)\n";
        std::cout << " |11> outcome : " << count11 << " (" << p11 << "%)\n";
        std::cout << " Invalid states : " << invalid_states << "\n";

        CHECK(invalid_states == 0);
        CHECK(p00 > 48.5 && p00 < 51.5);
        CHECK(p11 > 48.5 && p11 < 51.5);

        std::cout << " Statistical Monte Carlo Sampling: OK\n";
    }

    // -------------------------------------------------------
    // 5. Wavefunction Collapse on Partial Measurement
    // -------------------------------------------------------
    SECTION("5. Wavefunction Collapse & Partial Measurement")
    {
        State s(2);
        s.apply(gates::H(), 0);
        s.apply_cnot(0, 1);

        RNG rng(987654321UL);

        int c0 = 0, c1 = 0;
        const int N = 10000;

        for (int i = 0; i < N; ++i) {
            State temp = s;
            int bit = measure_qubit(temp, 0, rng);

            if (bit == 0) {
                ++c0;
                // Post-measurement state must collapse to |00>
                CHECK(std::abs(temp.amp[0].re - 1.0) < 1e-10);
                CHECK(temp.amp[1].norm_sq() < 1e-20);
                CHECK(temp.amp[2].norm_sq() < 1e-20);
                CHECK(temp.amp[3].norm_sq() < 1e-20);
            } else {
                ++c1;
                // Post-measurement state must collapse to |11>
                CHECK(std::abs(temp.amp[3].re - 1.0) < 1e-10);
            }
        }

        std::cout << " measure_qubit(0) -> 0: " << c0 << " | 1: " << c1 << "\n";
        CHECK(c0 + c1 == N);
        CHECK(std::abs(c0 - c1) < N * 0.05);

        std::cout << " Partial Measurement Collapse: OK\n";
    }

    // -------------------------------------------------------
    // 6. Multi-Qubit Entanglement (GHZ-4 State)
    // -------------------------------------------------------
    SECTION("6. Entanglement Verification (GHZ-4 State)")
    {
        State ghz(4);
        ghz.apply(gates::H(), 0);
        ghz.apply_cnot(0, 1);
        ghz.apply_cnot(0, 2);
        ghz.apply_cnot(0, 3);

        // (|0000> + |1111>) / sqrt(2)
        CHECK(std::abs(ghz.amp[0].re - INV_SQRT2) < 1e-10);
        CHECK(std::abs(ghz.amp[15].re - INV_SQRT2) < 1e-10);

        double norm_intermediate = 0.0;
        for (unsigned i = 1; i < 15; ++i) {
            norm_intermediate += ghz.amp[i].norm_sq();
        }
        CHECK(norm_intermediate < 1e-20);

        std::cout << " GHZ-4 Entanglement Verification: OK\n";
    }

    // -------------------------------------------------------
    // 7. High-Throughput Gate Benchmarking
    // -------------------------------------------------------
    SECTION("7. Execution Throughput & Stress Benchmark")
    {
        // 8 qubits = 256 amplitudes — комфортно для pure C++ без SIMD
        constexpr unsigned qubits = 8;
        constexpr unsigned gate_depth = 2000;
        const unsigned total_gates = qubits * gate_depth * 2; // H + CNOT

        State s(qubits);

        auto t0 = Clock::now();

        for (unsigned layer = 0; layer < gate_depth; ++layer) {
            for (unsigned q = 0; q < qubits; ++q) {
                s.apply(gates::H(), q);
                s.apply_cnot(q, (q + 1) % qubits);
            }
        }

        auto t1 = Clock::now();
        double total_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double gates_per_sec = total_gates / (total_ms / 1000.0);
        double ns_per_gate = (total_ms * 1e6) / total_gates;

        std::cout << " State Vector Size : " << qubits << " Qubits (" << (1u << qubits) << " amplitudes)\n";
        std::cout << " Evaluated Gates : " << total_gates << " operations\n";
        std::cout << " Execution Time : " << std::fixed << std::setprecision(3) << total_ms << " ms\n";
        std::cout << " Average Latency : " << std::fixed << std::setprecision(1) << ns_per_gate << " ns/gate\n";
        std::cout << " Throughput Rate : " << std::scientific << std::setprecision(2) << gates_per_sec << " ops/sec\n";

        // Validate state vector norm integrity after heavy transformation
        double norm = 0.0;
        for (unsigned i = 0; i < s.dim(); ++i) {
            norm += s.amp[i].norm_sq();
        }
        CHECK(std::abs(norm - 1.0) < 1e-7);

        std::cout << " High-Throughput Benchmark: OK\n";
    }

    // -------------------------------------------------------
    // 8. Normalization Stress Under Amplitude Drift
    // -------------------------------------------------------
    SECTION("8. Wavefunction Re-normalization Stress")
    {
        State s(4);
        for (unsigned q = 0; q < 4; ++q) {
            s.apply(gates::H(), q);
        }

        // Intentionally inject state amplitude scaling factor
        for (unsigned i = 0; i < s.dim(); ++i) {
            s.amp[i].re *= 14.88;
            s.amp[i].im *= 14.88;
        }

        s.normalize();

        double norm = 0.0;
        for (unsigned i = 0; i < s.dim(); ++i) {
            norm += s.amp[i].norm_sq();
        }
        CHECK(std::abs(norm - 1.0) < 1e-10);

        std::cout << " State Re-normalization: OK\n";
    }

    std::cout << "\n====================================================\n";
    std::cout << "All execution integration tests completed successfully!\n";
    return 0;
}
