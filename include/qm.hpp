#ifndef QM_HPP
#define QM_HPP

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
using Scalar = Complex;
using Real   = double;

// --- Gate & Operator Aliases ---
using QubitGate    = Matrix2x2;
using TwoQubitGate = Matrix4x4;
using Gate1Q       = Matrix2x2;
using Gate2Q       = Matrix4x4;
using Operator2x2  = Matrix2x2;
using Operator4x4  = Matrix4x4;

// --- Container & Buffer Aliases ---
using ComplexBuffer = Array;
using ComplexVector = Vector;
using GateSequence  = InstructionArray;

// --- Quantum Core Aliases ---
using QubitIndex = unsigned;
using BitCount   = unsigned;
using RegSize    = unsigned;
using GateType   = InstructionType;
using GateInst   = Instruction;

// --- Quantum State Aliases ---
using Wavefunction = State;

// --- Measurement Aliases ---
using ShotCount   = size_t;
using BitString   = size_t;
using ProbValue   = double;

} // namespace qm

#endif // QM_HPP
