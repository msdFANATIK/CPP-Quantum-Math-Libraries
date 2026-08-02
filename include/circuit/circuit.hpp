#ifndef QM_CIRCUIT_HPP
#define QM_CIRCUIT_HPP

#include "instruction.hpp"
#include "../state/statevector.hpp" 

namespace qm {

/**
 * @brief High-level quantum circuit container managing instruction streams.
 * 
 * Stores the number of qubits and a dynamic sequence of quantum gate instructions,
 * providing capabilities to build, modify, and execute circuits on quantum statevectors.
 */
class Circuit {
private:
    unsigned num_qubits;
        InstructionArray instructions;

public:
    /**
     * @brief Constructs a quantum circuit for a specified number of qubits.
     * 
     * @param qubits Total number of qubits in the circuit.
     */
    explicit Circuit(unsigned qubits) noexcept 
        : num_qubits(qubits), instructions() {}

    /**
     * @brief Constructs a quantum circuit with pre-allocated gate capacity.
     * 
     * @param qubits Total number of qubits in the circuit.
     * @param reserve_gates Initial pre-allocated capacity for instructions.
     */
    Circuit(unsigned qubits, size_t reserve_gates) 
        : num_qubits(qubits), instructions(reserve_gates) {}

    /**
     * @brief Appends a single-qubit gate instruction to the circuit.
     * 
     * @param target Target qubit index.
     * @param m      2x2 operator matrix.
     */
    void apply(unsigned target, const Matrix2x2& m) {
        instructions.push_back(Instruction::make_1q(target, m));
    }

    /**
     * @brief Appends a two-qubit gate instruction to the circuit.
     * 
     * @param control Control qubit index.
     * @param target  Target qubit index.
     * @param m       4x4 operator matrix.
     */
    void apply_two(unsigned control, unsigned target, const Matrix4x4& m) {
        instructions.push_back(Instruction::make_2q(control, target, m));
    }

    /**
     * @brief Appends a three-qubit gate instruction to the circuit.
     * 
     * @param control1 First control qubit index.
     * @param control2 Second control qubit index (or target secondary).
     * @param target   Target qubit index.
     * @param m        8x8 operator matrix.
     */
    void apply_three(unsigned control1, unsigned control2, unsigned target, const Matrix8x8& m) {
        instructions.push_back(Instruction::make_3q(control1, control2, target, m));
    }

    /**
     * @brief Reserves memory capacity for instructions.
     * 
     * @param gate_count Target gate capacity.
     */
    void reserve(size_t gate_count) {
        instructions.reserve(gate_count);
    }

    /**
     * @brief Executes the circuit on a statevector without measurement steps.
     * 
     * Iterates through all registered instructions sequentially, applying 
     * 1-qubit, 2-qubit, or 3-qubit gates to the statevector in-place.
     * 
     * @param state Target quantum statevector (modified in-place).
     * @return true if execution succeeded, false on qubit count mismatch.
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
            } else if (inst.type == InstructionType::Gate3Q) {
                state.apply(inst.gate.gate3, inst.control, inst.target);
            }
        }
        return true;
    }

    /**
     * @brief Returns a constant reference to the internal instruction array.
     */
    const InstructionArray& get_instructions() const noexcept {
        return instructions;
    }

    /**
     * @brief Returns the total number of qubits handled by this circuit.
     */
    unsigned get_num_qubits() const noexcept {
        return num_qubits;
    }

    /**
     * @brief Clears all instructions from the circuit.
     */
    void clear() noexcept {
        instructions.clear();
    }
};

} // namespace qm

#endif // QM_CIRCUIT_HPP
