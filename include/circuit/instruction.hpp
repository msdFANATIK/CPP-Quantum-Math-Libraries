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
    Gate2Q, ///< Two-qubit instruction (4x4 matrix)
    Gate3Q  ///< Three-qubit instruction (8x8 matrix)
};

/**
 * @brief Represents a single quantum gate instruction.
 */
struct alignas(64) Instruction {
    InstructionType type;
    unsigned target;
    unsigned control;
    unsigned control2; // Додатковий контрол для 3-кубітних вентилів

    union GateData {
        Matrix2x2 gate1;
        Matrix4x4 gate2;
        Matrix8x8 gate3;

        GateData() noexcept {}
        ~GateData() noexcept {}
    } gate;

    Instruction() noexcept 
        : type(InstructionType::Gate1Q), target(0), control(0), control2(0) {}

    ~Instruction() noexcept {}

    Instruction(const Instruction& other) noexcept 
        : type(other.type), target(other.target), control(other.control), control2(other.control2) {
        if (type == InstructionType::Gate1Q) {
            gate.gate1 = other.gate.gate1;
        } else if (type == InstructionType::Gate2Q) {
            gate.gate2 = other.gate.gate2;
        } else {
            gate.gate3 = other.gate.gate3;
        }
    }

    Instruction& operator=(const Instruction& other) noexcept {
        if (this != &other) {
            type = other.type;
            target = other.target;
            control = other.control;
            control2 = other.control2;
            if (type == InstructionType::Gate1Q) {
                gate.gate1 = other.gate.gate1;
            } else if (type == InstructionType::Gate2Q) {
                gate.gate2 = other.gate.gate2;
            } else {
                gate.gate3 = other.gate.gate3;
            }
        }
        return *this;
    }

    static Instruction make_1q(unsigned target_qubit, const Matrix2x2& m) noexcept {
        Instruction inst;
        inst.type = InstructionType::Gate1Q;
        inst.target = target_qubit;
        inst.control = 0;
        inst.control2 = 0;
        inst.gate.gate1 = m;
        return inst;
    }

    static Instruction make_2q(unsigned control_qubit, unsigned target_qubit, const Matrix4x4& m) noexcept {
        Instruction inst;
        inst.type = InstructionType::Gate2Q;
        inst.target = target_qubit;
        inst.control = control_qubit;
        inst.control2 = 0;
        inst.gate.gate2 = m;
        return inst;
    }

    static Instruction make_3q(unsigned c1, unsigned c2, unsigned target_qubit, const Matrix8x8& m) noexcept {
        Instruction inst;
        inst.type = InstructionType::Gate3Q;
        inst.target = target_qubit;
        inst.control = c1;
        inst.control2 = c2;
        inst.gate.gate3 = m;
        return inst;
    }
};

/**
 * @brief Dynamic contiguous array for Instruction structures.
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
    InstructionArray() noexcept : data_ptr(nullptr), capacity_val(0), size_val(0) {}

    explicit InstructionArray(size_t initial_capacity) 
        : data_ptr(initial_capacity > 0 ? new Instruction[initial_capacity] : nullptr), 
          capacity_val(initial_capacity), size_val(0) {}

    ~InstructionArray() noexcept { delete[] data_ptr; }

    InstructionArray(const InstructionArray& other) 
        : data_ptr(nullptr), capacity_val(other.capacity_val), size_val(other.size_val) {
        if (capacity_val > 0) {
            data_ptr = new Instruction[capacity_val];
            for (size_t i = 0; i < size_val; ++i) data_ptr[i] = other.data_ptr[i];
        }
    }

    InstructionArray& operator=(const InstructionArray& other) {
        if (this != &other) {
            Instruction* new_data = nullptr;
            if (other.capacity_val > 0) {
                new_data = new Instruction[other.capacity_val];
                for (size_t i = 0; i < other.size_val; ++i) new_data[i] = other.data_ptr[i];
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
        other.data_ptr = nullptr; other.capacity_val = 0; other.size_val = 0;
    }

    InstructionArray& operator=(InstructionArray&& other) noexcept {
        if (this != &other) {
            delete[] data_ptr;
            data_ptr = other.data_ptr;
            capacity_val = other.capacity_val;
            size_val = other.size_val;
            other.data_ptr = nullptr; other.capacity_val = 0; other.size_val = 0;
        }
        return *this;
    }

    void push_back(const Instruction& inst) {
        if (size_val >= capacity_val) {
            reallocate((capacity_val == 0) ? 8 : capacity_val * 2);
        }
        data_ptr[size_val++] = inst;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_val) reallocate(new_capacity);
    }

    void clear() noexcept { size_val = 0; }

    size_t size() const noexcept { return size_val; }
    size_t capacity() const noexcept { return capacity_val; }

    Instruction& operator[](size_t index) noexcept { return data_ptr[index]; }
    const Instruction& operator[](size_t index) const noexcept { return data_ptr[index]; }

    Instruction* begin() noexcept { return data_ptr; }
    Instruction* end() noexcept { return data_ptr + size_val; }
    const Instruction* begin() const noexcept { return data_ptr; }
    const Instruction* end() const noexcept { return data_ptr + size_val; }
};

} // namespace qm

#endif // QM_INSTRUCTION_HPP
