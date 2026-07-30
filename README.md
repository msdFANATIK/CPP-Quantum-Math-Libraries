# CPP Quantum Math Libraries

Modern C++20 header-only quantum simulation library.

A lightweight framework for quantum state manipulation, quantum gates, circuits, and measurements.  
Built from scratch with zero external dependencies and a custom math core.

## Features

- Statevector simulation
- Custom complex number implementation
- Quantum gates:
  - Pauli gates
  - Hadamard
  - Phase / Rotation gates
  - CNOT, CZ, SWAP
- Quantum circuit support
- Measurement and probability calculation
- Custom dynamic array backend
- Optional OpenMP acceleration
- Pure C++20, header-only
- Zero dependencies 

## Installation

This library is header-only. No compilation or linking is required.

Clone the repository:

```bash
git clone https://github.com/msdFANATIK/CPP-Quantum-Math-Libraries.git
```

Add the include directory to your compiler include path:

**Linux, GCC/Clang**
```bash
sudo cp -r CPP-Quantum-Math-Libraries/include/* /usr/local/include/
```

**Windows, MSVC**
```bash
setx INCLUDE "%INCLUDE%;C:\path\to\CPP-Quantum-Math-Libraries\include"
```
**Restart your terminal!**

Then include the library:
```cpp
#include <qm.hpp>
```
