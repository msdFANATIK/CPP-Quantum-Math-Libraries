#ifndef QM_CIRCUIT_HPP
#define QM_CIRCUIT_HPP

#include "instruction.hpp"
#include "../state/statevector.hpp" 

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

    /**
     * @brief Executes the circuit on a statevector without measurement steps.
     * @param state Target quantum statevector (modified in-place)
     * @return true if execution succeeded, false on qubit mismatch.
     */
    bool run_unitary(State& state) const noexcept {
        if (state.num_qubits() != num_qubits) {
            return false;
        }

        const size_t sz = instructions.size();
        for (size_t i = 0; i < sz; ++i) {
            const Instruction& inst = instructions[i];
            if (inst.type == InstructionType::Gate1Q) {
                state.apply(inst.gate.gate1, inst.target);
            } else if (inst.type == InstructionType::Gate2Q) {
                state.apply(inst.gate.gate2, inst.control, inst.target);
            }
        }
        return true;
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
