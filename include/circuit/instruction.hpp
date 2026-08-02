#ifndef QM_INSTRUCTION_HPP
#define QM_INSTRUCTION_HPP

#include "../core/complex.hpp"
#include "../gates/matrix.hpp"

namespace qm {

using size_t = decltype(sizeof(0));

/**
 * @brief Defines the type of quantum gate instruction.
 */
enum class InstructionType : unsigned char {
    Gate1Q, ///< Single-qubit instruction (2x2 matrix)
    Gate2Q  ///< Two-qubit instruction (4x4 matrix)
};

/**
 * @brief Represents a single quantum gate instruction.
 * 
 * Aligned to 64 bytes to match standard L1 cache line sizes, optimizing 
 * sequential instruction fetching and execution performance.
 */
struct alignas(64) Instruction {
    InstructionType type; ///< Type of the gate operation.
    unsigned target;      ///< Target qubit index.
    unsigned control;     ///< Control qubit index.

    /**
     * @brief Union holding the matrix representation based on the gate type.
     */
    union GateData {
        Matrix2x2 gate1;
        Matrix4x4 gate2;

        GateData() noexcept {}
        ~GateData() noexcept {}
    } gate;

    /**
     * @brief Constructs a default 1-qubit instruction.
     */
    Instruction() noexcept 
        : type(InstructionType::Gate1Q), target(0), control(0) {}

    ~Instruction() noexcept {}

    /**
     * @brief Copy constructor for Instruction.
     */
    Instruction(const Instruction& other) noexcept 
        : type(other.type), target(other.target), control(other.control) {
        if (type == InstructionType::Gate1Q) {
            gate.gate1 = other.gate.gate1;
        } else {
            gate.gate2 = other.gate.gate2;
        }
    }

    /**
     * @brief Copy assignment operator for Instruction.
     */
    Instruction& operator=(const Instruction& other) noexcept {
        if (this != &other) {
            type = other.type;
            target = other.target;
            control = other.control;
            if (type == InstructionType::Gate1Q) {
                gate.gate1 = other.gate.gate1;
            } else {
                gate.gate2 = other.gate.gate2;
            }
        }
        return *this;
    }

    /**
     * @brief Factory method to create a 1-qubit instruction.
     */
    static Instruction make_1q(unsigned target_qubit, const Matrix2x2& m) noexcept {
        Instruction inst;
        inst.type = InstructionType::Gate1Q;
        inst.target = target_qubit;
        inst.control = 0;
        inst.gate.gate1 = m;
        return inst;
    }

    /**
     * @brief Factory method to create a 2-qubit instruction.
     */
    static Instruction make_2q(unsigned control_qubit, unsigned target_qubit, const Matrix4x4& m) noexcept {
        Instruction inst;
        inst.type = InstructionType::Gate2Q;
        inst.target = target_qubit;
        inst.control = control_qubit;
        inst.gate.gate2 = m;
        return inst;
    }
};

/**
 * @brief Dynamic contiguous array for Instruction structures.
 * Zero external standard library dependencies.
 */
class InstructionArray {
private:
    Instruction* data_ptr;
    size_t capacity_val;
    size_t size_val;

    void reallocate(size_t new_capacity) {
        Instruction* new_data = new Instruction[new_capacity];
        for (size_t i = 0; i < size_val; ++i) {
            new_data[i] = data_ptr[i];
        }
        delete[] data_ptr;
        data_ptr = new_data;
        capacity_val = new_capacity;
    }

public:
    InstructionArray() noexcept 
        : data_ptr(nullptr), capacity_val(0), size_val(0) {}

    explicit InstructionArray(size_t initial_capacity) 
        : data_ptr(initial_capacity > 0 ? new Instruction[initial_capacity] : nullptr), 
          capacity_val(initial_capacity), 
          size_val(0) {}

    ~InstructionArray() noexcept {
        delete[] data_ptr;
    }

    InstructionArray(const InstructionArray& other) 
        : data_ptr(nullptr), capacity_val(other.capacity_val), size_val(other.size_val) {
        if (capacity_val > 0) {
            data_ptr = new Instruction[capacity_val];
            for (size_t i = 0; i < size_val; ++i) {
                data_ptr[i] = other.data_ptr[i];
            }
        }
    }

    InstructionArray& operator=(const InstructionArray& other) {
        if (this != &other) {
            Instruction* new_data = nullptr;
            if (other.capacity_val > 0) {
                new_data = new Instruction[other.capacity_val];
                for (size_t i = 0; i < other.size_val; ++i) {
                    new_data[i] = other.data_ptr[i];
                }
            }
            delete[] data_ptr;
            data_ptr = new_data;
            capacity_val = other.capacity_val;
            size_val = other.size_val;
        }
        return *this;
    }

    InstructionArray(InstructionArray&& other) noexcept 
        : data_ptr(other.data_ptr), capacity_val(other.capacity_val), size_val(other.size_val) {
        other.data_ptr = nullptr;
        other.capacity_val = 0;
        other.size_val = 0;
    }

    InstructionArray& operator=(InstructionArray&& other) noexcept {
        if (this != &other) {
            delete[] data_ptr;
            
            data_ptr = other.data_ptr;
            capacity_val = other.capacity_val;
            size_val = other.size_val;
            
            other.data_ptr = nullptr;
            other.capacity_val = 0;
            other.size_val = 0;
        }
        return *this;
    }

    void push_back(const Instruction& inst) {
        if (size_val >= capacity_val) {
            size_t next_capacity = (capacity_val == 0) ? 8 : capacity_val * 2;
            reallocate(next_capacity);
        }
        data_ptr[size_val++] = inst;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_val) {
            reallocate(new_capacity);
        }
    }

    void clear() noexcept {
        size_val = 0;
    }

    size_t size() const noexcept { return size_val; }
    size_t capacity() const noexcept { return capacity_val; }

    Instruction& operator[](size_t index) noexcept {
        return data_ptr[index];
    }

    const Instruction& operator[](size_t index) const noexcept {
        return data_ptr[index];
    }

    Instruction* begin() noexcept { return data_ptr; }
    Instruction* end() noexcept { return data_ptr + size_val; }
    const Instruction* begin() const noexcept { return data_ptr; }
    const Instruction* end() const noexcept { return data_ptr + size_val; }
};

} // namespace qm

#endif // QM_INSTRUCTION_HPP
