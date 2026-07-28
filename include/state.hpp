#pragma once 

#include "gates.hpp"

namespace qm {

struct State {
    Array amp;
    unsigned n;

    explicit State(unsigned num_qubits) : amp(1u << num_qubits), n(num_qubits) {
        if (amp.size > 0) {
            amp[0] = {1.0, 0.0}; // |0...0⟩
        }
    }

    void normalize() noexcept {
        double norm2 = 0.0;
        for (unsigned i = 0; i < amp.size; ++i) {
            norm2 += amp[i].norm_sq();
        }

        if (norm2 <= 0.0) return;

        const double inv = inv_sqrt(norm2);
        for (unsigned i = 0; i < amp.size; ++i) {
            amp[i].re *= inv;
            amp[i].im *= inv;
        }
    }

    void apply_direct(const Matrix2x2& gate, unsigned target) noexcept {
        const unsigned mask = 1u << target;
        const unsigned size = amp.size;

        for (unsigned i = 0; i < size; ++i) {
            if (i & mask) continue;

            const unsigned j = i | mask;
            const Complex a = amp[i]; // |...0...⟩
            const Complex b = amp[j]; // |...1...⟩

            amp[i] = gate.apply_alpha(a, b);
            amp[j] = gate.apply_beta(a, b);
        }
    }

    void apply(const Matrix2x2& gate, unsigned target) noexcept {
        if (target >= n) return;

        const unsigned mask = 1u << target;
        const unsigned size = amp.size;

        for (unsigned i = 0; i < size; ++i) {
            if (i & mask) continue;

            const unsigned j = i | mask;
            const Complex a = amp[i]; // |...0...⟩
            const Complex b = amp[j]; // |...1...⟩

            amp[i] = gate.apply_alpha(a, b);
            amp[j] = gate.apply_beta(a, b);
        }
    }

    void apply_direct(const Matrix4x4& gate, unsigned control, unsigned target) noexcept {
        Array temp(amp.size);
        temp.fill({0.0, 0.0});

        const unsigned size = amp.size;

        for (unsigned i = 0; i < size; ++i) {
            const unsigned c = (i >> control) & 1u;
            const unsigned t = (i >> target)  & 1u;
            const unsigned idx = (c << 1) | t; // 0..3

            for (unsigned j = 0; j < 4; ++j) {
                if (gate.m[j][idx].norm_sq() == 0.0) continue;

                unsigned new_i = i;

                new_i &= ~(1u << control);
                new_i &= ~(1u << target);

                if (j & 2u) new_i |= (1u << control);
                if (j & 1u) new_i |= (1u << target);

                temp[new_i] += gate.m[j][idx] * amp[i];
            }
        }

        amp = static_cast<Array&&>(temp); // move
    }

    void apply(const Matrix4x4& gate, unsigned control, unsigned target) noexcept {
        if (control >= n || target >= n || control == target) return;

        Array temp(amp.size);
        temp.fill({0.0, 0.0});

        const unsigned size = amp.size;

        for (unsigned i = 0; i < size; ++i) {
            const unsigned c = (i >> control) & 1u;
            const unsigned t = (i >> target)  & 1u;
            const unsigned idx = (c << 1) | t; // 0..3

            for (unsigned j = 0; j < 4; ++j) {
                if (gate.m[j][idx].norm_sq() == 0.0) continue;

                unsigned new_i = i;
                new_i &= ~(1u << control);
                new_i &= ~(1u << target);

                if (j & 2u) new_i |= (1u << control);
                if (j & 1u) new_i |= (1u << target);

                temp[new_i] += gate.m[j][idx] * amp[i];
            }
        }

        amp = static_cast<Array&&>(temp); // move
    }

    void apply_cnot(unsigned control, unsigned target) noexcept {
        if (control >= n || target >= n || control == target) return;

        const unsigned c_mask = 1u << control;
        const unsigned t_mask = 1u << target;
        const unsigned size = amp.size;

        for (unsigned i = 0; i < size; ++i) {
            if ((i & c_mask) && !(i & t_mask)) {
                const unsigned j = i | t_mask;
                const Complex temp_amp = amp[i];
                amp[i] = amp[j];
                amp[j] = temp_amp;
            }
        }
    }

    void apply_cz(unsigned control, unsigned target) noexcept {
        const unsigned mask = (1u << control) | (1u << target);
        const unsigned size = amp.size;

        for (unsigned i = 0; i < size; ++i) {
            if ((i & mask) == mask) {
                amp[i].re = -amp[i].re;
                amp[i].im = -amp[i].im;
            }
        }
    }

    unsigned num_qubits() const noexcept { return n; }
    unsigned dim() const noexcept { return amp.size; }
};

} // namespace qm
