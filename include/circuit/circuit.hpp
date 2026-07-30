#ifndef QM_CIRCUIT_HPP
#define QM_CIRCUIT_HPP

#include "instruction.hpp"

namespace qm {

/**
 * @brief High-level quantum circuit container managing instruction streams.
 */
class Circuit {
private:
    unsigned num_qubits;
    InstructionArray instructions;

public:
    explicit Circuit(unsigned qubits) noexcept 
        : num_qubits(qubits), instructions() {}

    Circuit(unsigned qubits, size_t reserve_gates) 
        : num_qubits(qubits), instructions(reserve_gates) {}

    void apply(unsigned target, const Matrix2x2& m) {
        instructions.push_back(Instruction::make_1q(target, m));
    }

    void apply_two(unsigned control, unsigned target, const Matrix4x4& m) {
        instructions.push_back(Instruction::make_2q(control, target, m));
    }

    void reserve(size_t gate_count) {
        instructions.reserve(gate_count);
    }

    const InstructionArray& get_instructions() const noexcept {
        return instructions;
    }

    unsigned get_num_qubits() const noexcept {
        return num_qubits;
    }

    void clear() noexcept {
        instructions.clear();
    }
};

} // namespace qm

#endif // QM_CIRCUIT_HPP
