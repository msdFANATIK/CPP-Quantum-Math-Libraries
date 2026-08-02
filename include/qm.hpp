#ifndef QM_HPP
#define QM_HPP

/**
 * @file qm.hpp
 * @brief Master include header and public type aliases for the Quantum Mechanics (QM) simulation framework.
 */

// Core Data Structures & Utilities
#include "core/complex.hpp"
#include "core/array.hpp"
#include "core/vector_api.hpp"
#include "math/utils.hpp"
#include "core/types.hpp"

// Math & Gates
#include "gates/matrix.hpp"
#include "gates/gates.hpp"

// Quantum Circuit Engine
#include "circuit/instruction.hpp"
#include "circuit/circuit.hpp"

// State Representation & Operations
#include "state/statevector.hpp"
#include "measure/measurement.hpp"

namespace qm {

// --- Scalar Types ---
using Scalar = Complex; ///< Primary complex number scalar type.
using Real   = double;  ///< Primary real number scalar type.

// --- Gate & Operator Aliases ---
using QubitGate      = Matrix2x2; ///< Single-qubit operator alias.
using TwoQubitGate   = Matrix4x4; ///< Two-qubit operator alias.
using ThreeQubitGate = Matrix8x8; ///< Three-qubit operator alias.
using Gate1Q         = Matrix2x2; ///< Single-qubit gate alias.
using Gate2Q         = Matrix4x4; ///< Two-qubit gate alias.
using Gate3Q         = Matrix8x8; ///< Three-qubit gate alias.
using Operator2x2    = Matrix2x2; ///< 2x2 matrix operator alias.
using Operator4x4    = Matrix4x4; ///< 4x4 matrix operator alias.
using Operator8x8    = Matrix8x8; ///< 8x8 matrix operator alias.

// --- Container & Buffer Aliases ---
using ComplexBuffer = Array;          ///< Dynamic raw complex storage buffer.
using ComplexVector = Vector;         ///< Dynamic vector alias.
using GateSequence  = InstructionArray; ///< Instruction stream container.

// --- Quantum Core Aliases ---
using QubitIndex = unsigned;      ///< Identifier type for individual qubits.
using BitCount   = unsigned;      ///< Type representing bit counts.
using RegSize    = unsigned;      ///< Register size type.
using GateType   = InstructionType; ///< Gate classification enum type.
using GateInst   = Instruction;   ///< Individual circuit instruction type.

// --- Quantum State Aliases ---
using Wavefunction = State; ///< Quantum state vector descriptor.

// --- Measurement Aliases ---
using ShotCount   = size_t; ///< Execution shots count type.
using BitString   = size_t; ///< Measured bitstring outcome value type.
using ProbValue   = double; ///< Probability representation type.

} // namespace qm

#endif // QM_HPP
