#pragma once

#include "types.hpp"

namespace qm {

// -------------------------------------------------------------------------
// Custom Type Definitions & Constants (Zero Stdlib)
// -------------------------------------------------------------------------
using size_t = unsigned long long;
using uintptr_t = unsigned long long;

static constexpr size_t ALIGNMENT = 64;

// -------------------------------------------------------------------------
// Direct OS Kernel Allocator (Linux Syscalls / Windows Native)
// -------------------------------------------------------------------------
namespace detail {

#if defined(_WIN32) || defined(_WIN64)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* lpAddress, size_t dwSize, unsigned long flAllocationType, unsigned long flProtect);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* lpAddress, size_t dwSize, unsigned long dwFreeType);

static inline void* os_alloc(size_t bytes) noexcept {
    // MEM_COMMIT | MEM_RESERVE = 0x1000 | 0x2000 = 0x3000, PAGE_READWRITE = 0x04
    return VirtualAlloc(nullptr, bytes, 0x3000, 0x04);
}

static inline void os_free(void* ptr, size_t) noexcept {
    // MEM_RELEASE = 0x8000
    if (ptr) VirtualFree(ptr, 0, 0x8000);
}
#elif defined(__x86_64__)
static inline void* os_alloc(size_t bytes) noexcept {
    // Linux sys_brk (Syscall 12)
    void* current_brk = nullptr;
    asm volatile(
        "mov $12, %%rax\n\t"
        "xor %%rdi, %%rdi\n\t"
        "syscall\n\t"
        "mov %%rax, %0"
        : "=r"(current_brk)
        :
        : "rax", "rdi", "rcx", "r11", "memory"
    );

    uintptr_t new_addr = reinterpret_cast<uintptr_t>(current_brk) + bytes;
    void* result_brk = nullptr;
    asm volatile(
        "mov $12, %%rax\n\t"
        "mov %1, %%rdi\n\t"
        "syscall\n\t"
        "mov %%rax, %0"
        : "=r"(result_brk)
        : "r"(new_addr)
        : "rax", "rdi", "rcx", "r11", "memory"
    );

    if (result_brk == current_brk) return nullptr;
    return current_brk;
}

static inline void os_free(void*, size_t) noexcept {
    // Baremetal arena allocation: stack/heap rollbacks omitted for max speed
}
#elif defined(__aarch64__)
static inline void* os_alloc(size_t bytes) noexcept {
    // Linux sys_brk for AArch64 (Syscall 214)
    void* current_brk = nullptr;
    asm volatile(
        "mov x8, #214\n\t"
        "mov x0, #0\n\t"
        "svc #0\n\t"
        "mov %0, x0"
        : "=r"(current_brk)
        :
        : "x0", "x8", "memory"
    );

    uintptr_t new_addr = reinterpret_cast<uintptr_t>(current_brk) + bytes;
    void* result_brk = nullptr;
    asm volatile(
        "mov x8, #214\n\t"
        "mov x0, %1\n\t"
        "svc #0\n\t"
        "mov %0, x0"
        : "=r"(result_brk)
        : "r"(new_addr)
        : "x0", "x8", "memory"
    );

    if (result_brk == current_brk) return nullptr;
    return current_brk;
}

static inline void os_free(void*, size_t) noexcept {}
#endif

/**
 * @brief Allocates raw memory block aligned to 64 bytes.
 * Stores original unaligned pointer right before the payload pointer.
 */
static inline Complex* raw_alloc_aligned(size_t count) noexcept {
    if (count == 0) return nullptr;

    size_t payload_bytes = count * sizeof(Complex);
    size_t total_bytes = payload_bytes + ALIGNMENT + sizeof(void*);

    void* raw_ptr = os_alloc(total_bytes);
    if (!raw_ptr) return nullptr;

    uintptr_t raw_addr = reinterpret_cast<uintptr_t>(raw_ptr) + sizeof(void*);
    uintptr_t aligned_addr = (raw_addr + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

    void** header = reinterpret_cast<void**>(aligned_addr) - 1;
    *header = raw_ptr;

    return reinterpret_cast<Complex*>(aligned_addr);
}

static inline void raw_free_aligned(Complex* ptr, size_t count) noexcept {
    if (!ptr) return;
    void* raw_ptr = *(reinterpret_cast<void**>(ptr) - 1);
    size_t total_bytes = (count * sizeof(Complex)) + ALIGNMENT + sizeof(void*);
    os_free(raw_ptr, total_bytes);
}

} // namespace detail

/**
 * @brief Zero-dependency, zero-stdlib high performance array for quantum data.
 */
struct Array {
    Complex* data_ptr{nullptr};
    size_t size_val{0};
    size_t capacity_val{0};

    // ---------------------------------------------------------------------
    // Constructors / Destructor
    // ---------------------------------------------------------------------

    Array() noexcept = default;

    explicit Array(size_t n) 
        : data_ptr(detail::raw_alloc_aligned(n)), size_val(n), capacity_val(n) {
        if (data_ptr) {
            fill_zero();
        }
    }

    Array(const Array& other) 
        : data_ptr(detail::raw_alloc_aligned(other.capacity_val)),
          size_val(other.size_val),
          capacity_val(other.capacity_val) {
        if (data_ptr && other.data_ptr) {
            for (size_t i = 0; i < size_val; ++i) {
                data_ptr[i] = other.data_ptr[i];
            }
        }
    }

    Array(Array&& other) noexcept
        : data_ptr(other.data_ptr), size_val(other.size_val), capacity_val(other.capacity_val) {
        other.data_ptr = nullptr;
        other.size_val = 0;
        other.capacity_val = 0;
    }

    ~Array() noexcept {
        detail::raw_free_aligned(data_ptr, capacity_val);
    }

    // ---------------------------------------------------------------------
    // Assignment
    // ---------------------------------------------------------------------

    Array& operator=(const Array& other) {
        if (this == &other) return *this;

        if (capacity_val >= other.size_val) {
            size_val = other.size_val;
            for (size_t i = 0; i < size_val; ++i) {
                data_ptr[i] = other.data_ptr[i];
            }
            return *this;
        }

        Complex* new_data = detail::raw_alloc_aligned(other.capacity_val);
        if (new_data && other.data_ptr) {
            for (size_t i = 0; i < other.size_val; ++i) {
                new_data[i] = other.data_ptr[i];
            }
        }

        detail::raw_free_aligned(data_ptr, capacity_val);
        data_ptr = new_data;
        size_val = other.size_val;
        capacity_val = other.capacity_val;
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this == &other) return *this;

        detail::raw_free_aligned(data_ptr, capacity_val);
        data_ptr = other.data_ptr;
        size_val = other.size_val;
        capacity_val = other.capacity_val;

        other.data_ptr = nullptr;
        other.size_val = 0;
        other.capacity_val = 0;
        return *this;
    }

    // ---------------------------------------------------------------------
    // Element Access
    // ---------------------------------------------------------------------

    Complex& operator[](size_t i) noexcept { return data_ptr[i]; }
    const Complex& operator[](size_t i) const noexcept { return data_ptr[i]; }

    Complex& front() noexcept { return data_ptr[0]; }
    const Complex& front() const noexcept { return data_ptr[0]; }

    Complex& back() noexcept { return data_ptr[size_val - 1]; }
    const Complex& back() const noexcept { return data_ptr[size_val - 1]; }

    Complex* data() noexcept { return data_ptr; }
    const Complex* data() const noexcept { return data_ptr; }

    // ---------------------------------------------------------------------
    // Iterators
    // ---------------------------------------------------------------------

    Complex* begin() noexcept { return data_ptr; }
    Complex* end() noexcept { return data_ptr + size_val; }

    const Complex* begin() const noexcept { return data_ptr; }
    const Complex* end() const noexcept { return data_ptr + size_val; }

    const Complex* cbegin() const noexcept { return data_ptr; }
    const Complex* cend() const noexcept { return data_ptr + size_val; }

    // ---------------------------------------------------------------------
    // Capacity & Operations
    // ---------------------------------------------------------------------

    bool empty() const noexcept { return size_val == 0; }
    size_t size() const noexcept { return size_val; }
    size_t capacity() const noexcept { return capacity_val; }

    void reserve(size_t new_cap) {
        if (new_cap <= capacity_val) return;

        Complex* new_data = detail::raw_alloc_aligned(new_cap);
        if (data_ptr) {
            for (size_t i = 0; i < size_val; ++i) {
                new_data[i] = data_ptr[i];
            }
            detail::raw_free_aligned(data_ptr, capacity_val);
        }

        data_ptr = new_data;
        capacity_val = new_cap;
    }

    void shrink_to_fit() {
        if (size_val == capacity_val) return;

        if (size_val == 0) {
            detail::raw_free_aligned(data_ptr, capacity_val);
            data_ptr = nullptr;
            capacity_val = 0;
            return;
        }

        Complex* new_data = detail::raw_alloc_aligned(size_val);
        for (size_t i = 0; i < size_val; ++i) {
            new_data[i] = data_ptr[i];
        }

        detail::raw_free_aligned(data_ptr, capacity_val);
        data_ptr = new_data;
        capacity_val = size_val;
    }

    void resize(size_t n, const Complex& value = Complex{0.0, 0.0}) {
        if (n <= capacity_val) {
            if (n > size_val) {
                for (size_t i = size_val; i < n; ++i) {
                    data_ptr[i] = value;
                }
            }
            size_val = n;
            return;
        }

        reserve(n);
        for (size_t i = size_val; i < n; ++i) {
            data_ptr[i] = value;
        }
        size_val = n;
    }

    void push_back(const Complex& val) {
        if (size_val >= capacity_val) {
            size_t next_cap = (capacity_val == 0) ? 8 : capacity_val * 2;
            reserve(next_cap);
        }
        data_ptr[size_val++] = val;
    }

    void fill(const Complex& value) noexcept {
        #pragma omp parallel for if(size_val >= 8192) schedule(static)
        for (size_t i = 0; i < size_val; ++i) {
            data_ptr[i] = value;
        }
    }

    void clear() noexcept {
        size_val = 0;
    }

    void swap(Array& other) noexcept {
        Complex* td = data_ptr;
        data_ptr = other.data_ptr;
        other.data_ptr = td;

        size_t ts = size_val;
        size_val = other.size_val;
        other.size_val = ts;

        size_t tc = capacity_val;
        capacity_val = other.capacity_val;
        other.capacity_val = tc;
    }

    void fill_zero() noexcept {
        if (!data_ptr || size_val == 0) return;

        #pragma omp parallel for if(size_val >= 8192) schedule(static)
        for (size_t i = 0; i < size_val; ++i) {
            data_ptr[i] = Complex{0.0, 0.0};
        }
    }
};

} // namespace qm
