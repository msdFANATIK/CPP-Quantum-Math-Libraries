# CPP Quantum Math Lib

**Modern C++20 header-only quantum simulation library**

Lightweight, zero-dependency framework for quantum state manipulation, gates, circuits and measurements.  
Built from scratch with a custom math core (no Eigen, no STL containers for the hot path).

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Header-only](https://img.shields.io/badge/header--only-yes-brightgreen.svg)]()

---

## Features

- Full **statevector** simulation
- Custom `Complex` and dynamic array backend (no external math libraries)
- Single- and two-qubit gates:
  - Pauli (X, Y, Z)
  - Hadamard
  - Phase / Rotation gates
  - CNOT, CZ, SWAP
- Quantum circuit abstraction
- Measurement with wavefunction collapse + probability calculation
- Optional **OpenMP** acceleration
- Pure **C++20**, header-only
- **Zero dependencies**

---

## Quick Start

```bash
git clone https://github.com/msdFANATIK/CPP-Quantum-Math-Lib.git
cd CPP-Quantum-Math-Lib
```

### Compile & run example

```bash
g++ -std=c++20 -O3 -Iinclude examples/hello_qm.cpp -o hello_qm
./hello_qm
```

### CMake

```bash
mkdir build && cd build
cmake -S ../examples -B .
cmake --build .
./hello_qm
```

---

## Installation / Usage

The library is header-only. Just add the `include` directory to your include path:

```bash
g++ -std=c++20 -I/path/to/CPP-Quantum-Math-Lib/include your_program.cpp -O3
```

Or with CMake (`FetchContent` recommended):

```cmake
include(FetchContent)
FetchContent_Declare(
  qm
  GIT_REPOSITORY https://github.com/msdFANATIK/CPP-Quantum-Math-Lib.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(qm)

target_include_directories(your_target PRIVATE ${qm_SOURCE_DIR}/include)
```

Then in code:

```cpp
#include <qm.hpp>
```

---

## Minimal Example

```cpp
#include <qm.hpp>
#include <iostream>

int main() {
    qm::State psi(2);               // |00⟩

    psi.apply(qm::gates::H(), 0);   // H on qubit 0
    psi.apply_cnot(0, 1);           // CNOT → Bell state

    std::cout << "Bell state amplitudes:\n";
    for (unsigned i = 0; i < psi.dim(); ++i) {
        std::cout << "|" << i << "⟩ : " << psi.amp[i] << "\n";
    }
}
```

---

## Project Structure

```
include/
├── qm.hpp                 # main header
├── core/                  # Complex, Array, types
├── gates/                 # matrices & gate definitions
├── state/                 # Statevector
├── math/                  # Mathematics 
├── measure/               # measurement & collapse
└── circuit/               # circuit abstraction
examples/
tests/
Benchmark/
```

---

## Why this library?

| Feature                  | CPP-Quantum-Math-Lib | Typical alternatives (qpp etc.) |
|--------------------------|----------------------|---------------------------------|
| External dependencies    | **0**                | Eigen / others                  |
| Header-only              | Yes                  | Yes                             |
| Custom math core         | Yes                  | No                              |
| C++ standard             | C++20                | C++17+                          |
| OpenMP support           | Yes                  | Usually yes                     |

Ideal when you want a minimal and good, self-contained quantum engine without pulling large linear-algebra libraries.

---

## License

MIT © msdFANATIK
