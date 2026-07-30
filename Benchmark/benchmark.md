# ⚡ Benchmark Results: CPP-Quantum-Math-Libraries (qm-qpp)

### Environment Configuration
* **Compiler:** GCC (C++20 Standard)
* **Optimization Flags:** `-O3 -march=native -ffast-math -flto -fopenmp`
* **Target Architecture:** x86_64 (SIMD/AVX Vectorized & OpenMP Multi-threaded)

---

### Core Execution Metrics

| Qubits | Statevector Dim | Gate Operations | Iterations | Avg Execution Time (CPU Ticks) |
| :---: | :---: | :---: | :---: | :---: |
| **4** | 16 | 11 | 10,000 | **13,473** |
| **10** | 1,024 | 29 | 1,000 | **186,911** |
| **14** | 16,384 | 41 | 500 | **4,399,042** |
| **18** | 262,144 | 53 | 50 | **109,888,491** |

---

### Console Output Log

```text
=== QUANTUM ENGINE SYSTEM BENCHMARK (qm-qpp) ===

==================================================
 QUANTUM BENCHMARK FOR: 4 QUBITS (Dimension: 16)
==================================================
 Circuit constructed. Total operations: 11
 Warming up CPU caches and allocating statevector...
 Executing 10000 benchmark iterations (zero malloc overhead)...
 -> Success! Average pure core execution time: 13473 CPU ticks

==================================================
 QUANTUM BENCHMARK FOR: 10 QUBITS (Dimension: 1024)
==================================================
 Circuit constructed. Total operations: 29
 Warming up CPU caches and allocating statevector...
 Executing 1000 benchmark iterations (zero malloc overhead)...
 -> Success! Average pure core execution time: 186911 CPU ticks

==================================================
 QUANTUM BENCHMARK FOR: 14 QUBITS (Dimension: 16384)
==================================================
 Circuit constructed. Total operations: 41
 Warming up CPU caches and allocating statevector...
 Executing 500 benchmark iterations (zero malloc overhead)...
 -> Success! Average pure core execution time: 4399042 CPU ticks

==================================================
 QUANTUM BENCHMARK FOR: 18 QUBITS (Dimension: 262144)
==================================================
 Circuit constructed. Total operations: 53
 Warming up CPU caches and allocating statevector...
 Executing 50 benchmark iterations (zero malloc overhead)...
 -> Success! Average pure core execution time: 109888491 CPU ticks

==================================================
Benchmark completed. Your engine is flying!
