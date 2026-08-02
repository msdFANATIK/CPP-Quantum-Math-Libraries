#pragma once

#include "../core/array.hpp"
#include "../gates/gates.hpp"

namespace qm {

/**
 * @brief Quantum statevector representation.
 *
 * Stores amplitudes of a pure quantum state in computational basis.
 * Number of qubits is fixed at construction time.
 */
struct State {
    Array amp;       ///< Amplitudes (size = 2^n)
    unsigned n;      ///< Number of qubits

    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    /**
     * @brief Creates state |0...0⟩ with @p num_qubits qubits.
     */
    explicit State(unsigned num_qubits)
        : amp(num_qubits < 32 ? (1u << num_qubits) : 0u), n(num_qubits)
    {
        if (!amp.empty()) {
            amp[0] = {1.0, 0.0};
        }
    }

    // ---------------------------------------------------------------------
    // Basic info
    // ---------------------------------------------------------------------

    unsigned num_qubits() const noexcept { return n; }
    unsigned dim()        const noexcept { return amp.get_size(); }
    bool     empty()      const noexcept { return amp.empty(); }

    // ---------------------------------------------------------------------
    // Amplitude access
    // ---------------------------------------------------------------------

    Complex&       operator[](unsigned i)        noexcept { return amp[i]; }
    const Complex& operator[](unsigned i) const noexcept { return amp[i]; }

    Complex  get(unsigned i) const noexcept { return amp[i]; }
    void     set(unsigned i, const Complex& v) noexcept { amp[i] = v; }

    // ---------------------------------------------------------------------
    // Norm & probability
    // ---------------------------------------------------------------------

    /** @brief Squared norm of the statevector. */
    double norm_sq() const noexcept {
        double sum = 0.0;
        const unsigned sz = amp.get_size();

        #pragma omp parallel for if(sz >= 8192) reduction(+:sum) schedule(static)
        for (unsigned i = 0; i < sz; ++i) {
            sum += amp[i].norm_sq();
        }
        return sum;
    }

    /** @brief Euclidean norm. */
    double norm() const noexcept {
        return sqrt(norm_sq());
    }

    /** @brief In-place normalization. */
    void normalize() noexcept {
        const double n2 = norm_sq();
        if (n2 <= 0.0) return;

        const double inv = inv_sqrt(n2);
        const unsigned sz = amp.get_size();

        #pragma omp parallel for if(sz >= 8192) schedule(static)
        for (unsigned i = 0; i < sz; ++i) {
            amp[i] *= inv;
        }
    }

    /** @brief Probability of measuring computational basis state @p outcome. */
    double probability(unsigned outcome) const noexcept {
        if (outcome >= amp.get_size()) return 0.0;
        return amp[outcome].norm_sq();
    }

    // ---------------------------------------------------------------------
    // Single-qubit gates
    // ---------------------------------------------------------------------

    /**
     * @brief Applies a single-qubit gate to target qubit.
     * @param gate   2×2 unitary matrix
     * @param target Target qubit index
     */
    void apply(const Matrix2x2& gate, unsigned target) noexcept {
        if (target >= n) return;
        apply_direct(gate, target);
    }

    /** @brief Fast version without bounds check. Branchless indexing. */
    void apply_direct(const Matrix2x2& gate, unsigned target) noexcept {
        const unsigned mask = 1u << target;
        const unsigned low_mask = mask - 1u;
        const unsigned high_mask = ~low_mask;
        const unsigned half_sz = amp.get_size() >> 1;

        #pragma omp parallel for if(half_sz >= 4096) schedule(static)
        for (unsigned i = 0; i < half_sz; ++i) {
            const unsigned k0 = ((i & high_mask) << 1) | (i & low_mask);
            const unsigned k1 = k0 | mask;

            const Complex a = amp[k0];
            const Complex b = amp[k1];

            amp[k0] = gate.apply_alpha(a, b);
            amp[k1] = gate.apply_beta(a, b);
        }
    }

    // ---------------------------------------------------------------------
    // Two-qubit gates (general)
    // ---------------------------------------------------------------------

    /**
     * @brief Applies a general two-qubit gate.
     * @param gate    4×4 unitary matrix
     * @param control Control qubit
     * @param target  Target qubit
     */
    void apply(const Matrix4x4& gate, unsigned control, unsigned target) noexcept {
        if (control >= n || target >= n || control == target) return;
        apply_direct(gate, control, target);
    }

    /** @brief Fast version without dynamic allocations (in-place block execution). */
    void apply_direct(const Matrix4x4& gate, unsigned control, unsigned target) noexcept {
        const unsigned min_q = control < target ? control : target;
        const unsigned max_q = control > target ? control : target;

        const unsigned m0 = (1u << min_q) - 1u;
        const unsigned m1 = ((1u << (max_q - 1u)) - 1u) ^ m0;
        const unsigned m2 = ~(m0 | m1 | (1u << min_q) | (1u << max_q));

        const unsigned c_mask = 1u << control;
        const unsigned t_mask = 1u << target;
        const unsigned quarter_sz = amp.get_size() >> 2;

        #pragma omp parallel for if(quarter_sz >= 2048) schedule(static)
        for (unsigned i = 0; i < quarter_sz; ++i) {
            const unsigned idx00 = (i & m0) | ((i & m1) << 1) | ((i & m2) << 2);
            const unsigned idx01 = idx00 | t_mask;
            const unsigned idx10 = idx00 | c_mask;
            const unsigned idx11 = idx00 | c_mask | t_mask;

            const Complex in[4] = {
                amp[idx00],
                amp[idx01],
                amp[idx10],
                amp[idx11]
            };

            Complex out[4];
            gate.apply(in, out);

            amp[idx00] = out[0];
            amp[idx01] = out[1];
            amp[idx10] = out[2];
            amp[idx11] = out[3];
        }
    }

    // ---------------------------------------------------------------------
    // Specialized two-qubit gates (fast paths)
    // ---------------------------------------------------------------------

    void apply_cnot(unsigned control, unsigned target) noexcept {
        if (control >= n || target >= n || control == target) return;

        const unsigned min_q = control < target ? control : target;
        const unsigned max_q = control > target ? control : target;

        const unsigned m0 = (1u << min_q) - 1u;
        const unsigned m1 = ((1u << (max_q - 1u)) - 1u) ^ m0;
        const unsigned m2 = ~(m0 | m1 | (1u << min_q) | (1u << max_q));

        const unsigned c_mask = 1u << control;
        const unsigned t_mask = 1u << target;
        const unsigned quarter_sz = amp.get_size() >> 2;

        #pragma omp parallel for if(quarter_sz >= 2048) schedule(static)
        for (unsigned i = 0; i < quarter_sz; ++i) {
            const unsigned base = (i & m0) | ((i & m1) << 1) | ((i & m2) << 2);
            const unsigned idx10 = base | c_mask;
            const unsigned idx11 = idx10 | t_mask;

            const Complex tmp = amp[idx10];
            amp[idx10] = amp[idx11];
            amp[idx11] = tmp;
        }
    }

    void apply_cz(unsigned control, unsigned target) noexcept {
        if (control >= n || target >= n || control == target) return;

        const unsigned min_q = control < target ? control : target;
        const unsigned max_q = control > target ? control : target;

        const unsigned m0 = (1u << min_q) - 1u;
        const unsigned m1 = ((1u << (max_q - 1u)) - 1u) ^ m0;
        const unsigned m2 = ~(m0 | m1 | (1u << min_q) | (1u << max_q));

        const unsigned c_mask = 1u << control;
        const unsigned t_mask = 1u << target;
        const unsigned quarter_sz = amp.get_size() >> 2;

        #pragma omp parallel for if(quarter_sz >= 2048) schedule(static)
        for (unsigned i = 0; i < quarter_sz; ++i) {
            const unsigned idx11 = (i & m0) | ((i & m1) << 1) | ((i & m2) << 2) | c_mask | t_mask;
            amp[idx11] = -amp[idx11];
        }
    }

    void apply_swap(unsigned q1, unsigned q2) noexcept {
        if (q1 >= n || q2 >= n || q1 == q2) return;

        const unsigned min_q = q1 < q2 ? q1 : q2;
        const unsigned max_q = q1 > q2 ? q1 : q2;

        const unsigned m0 = (1u << min_q) - 1u;
        const unsigned m1 = ((1u << (max_q - 1u)) - 1u) ^ m0;
        const unsigned m2 = ~(m0 | m1 | (1u << min_q) | (1u << max_q));

        const unsigned m1_mask = 1u << q1;
        const unsigned m2_mask = 1u << q2;
        const unsigned quarter_sz = amp.get_size() >> 2;

        #pragma omp parallel for if(quarter_sz >= 2048) schedule(static)
        for (unsigned i = 0; i < quarter_sz; ++i) {
            const unsigned base = (i & m0) | ((i & m1) << 1) | ((i & m2) << 2);
            const unsigned idx10 = base | m1_mask;
            const unsigned idx01 = base | m2_mask;

            const Complex tmp = amp[idx10];
            amp[idx10] = amp[idx01];
            amp[idx01] = tmp;
        }
    }

    // ---------------------------------------------------------------------
    // Three-qubit gates (general)
    // ---------------------------------------------------------------------

    /**
     * @brief Applies a general three-qubit gate.
     * @param gate     8×8 unitary matrix
     * @param control1 First control qubit
     * @param control2 Second control qubit
     * @param target   Target qubit
     */
    void apply(const Matrix8x8& gate, unsigned control1, unsigned control2, unsigned target) noexcept {
        if (control1 >= n || control2 >= n || target >= n || 
            control1 == control2 || control1 == target || control2 == target) return;
        apply_direct(gate, control1, control2, target);
    }

    /** @brief Fast version without dynamic allocations for 3-qubit gates. */
    void apply_direct(const Matrix8x8& gate, unsigned control1, unsigned control2, unsigned target) noexcept {
        // Find the order of qubits for correct bit masking
        unsigned q[3] = {control1, control2, target};
        // Simple bubble sort for three elements
        if (q[0] > q[1]) { unsigned t = q[0]; q[0] = q[1]; q[1] = t; }
        if (q[1] > q[2]) { unsigned t = q[1]; q[1] = q[2]; q[2] = t; }
        if (q[0] > q[1]) { unsigned t = q[0]; q[0] = q[1]; q[1] = t; }

        unsigned q0 = q[0], q1 = q[1], q2 = q[2];

        const unsigned m0 = (1u << q0) - 1u;
        const unsigned m1 = ((1u << (q1 - 1u)) - 1u) ^ m0;
        const unsigned m2 = ((1u << (q2 - 2u)) - 1u) ^ m0 ^ m1;
        const unsigned m3 = ~(m0 | m1 | m2 | (1u << q0) | (1u << q1) | (1u << q2));

        const unsigned mask_c1 = 1u << control1;
        const unsigned mask_c2 = 1u << control2;
        const unsigned mask_t  = 1u << target;
        
        const unsigned eighth_sz = amp.get_size() >> 3;

        #pragma omp parallel for if(eighth_sz >= 1024) schedule(static)
        for (unsigned i = 0; i < eighth_sz; ++i) {
            const unsigned base = (i & m0) | ((i & m1) << 1) | ((i & m2) << 2) | ((i & m3) << 3);

            unsigned idx[8];
            idx[0] = base;
            idx[1] = base | mask_t;
            idx[2] = base | mask_c2;
            idx[3] = base | mask_c2 | mask_t;
            idx[4] = base | mask_c1;
            idx[5] = base | mask_c1 | mask_t;
            idx[6] = base | mask_c1 | mask_c2;
            idx[7] = base | mask_c1 | mask_c2 | mask_t;

            Complex in[8];
            for (int j = 0; j < 8; ++j) {
                in[j] = amp[idx[j]];
            }

            Complex out[8];
            gate.apply(in, out);

            for (int j = 0; j < 8; ++j) {
                amp[idx[j]] = out[j];
            }
        }
    }

    // ---------------------------------------------------------------------
    // Reset state (Zero-allocation)
    // ---------------------------------------------------------------------

    /**
     * @brief Resets statevector to computational state |0...0⟩ without external dependencies.
     */
    void reset() noexcept {
        if (amp.empty()) return;

        amp.fill_zero();

        amp[0] = Complex{1.0, 0.0};
    }
};

} // namespace qm
