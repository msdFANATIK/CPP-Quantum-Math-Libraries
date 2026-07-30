#pragma once

#include "types.hpp"

namespace qm {

/**
 * @brief Low-level storage engine managing a dynamic array of Complex numbers.
 * 
 * Provides raw buffer allocation, reallocation logic, and basic memory management.
 */
struct Array {
    Complex* data{nullptr};
    unsigned size{0};
    unsigned capacity{0};

    // ---------------------------------------------------------------------
    // Constructors / Destructor
    // ---------------------------------------------------------------------

    /** @brief Constructs an empty array with zero capacity. */
    Array() noexcept = default;

    /**
     * @brief Constructs an array with @p n zero-initialized elements.
     * @param n Initial number of elements and capacity.
     */
    explicit Array(unsigned n) : data(nullptr), size(n), capacity(n) {
        if (n > 0) {
            data = new Complex[n]{};
        }
    }

    /** @brief Copy constructor. Performs a deep copy of the buffer. */
    Array(const Array& other) : data(nullptr), size(other.size), capacity(other.capacity) {
        if (capacity > 0) {
            data = new Complex[capacity]{}; // Zero-initialize the entire buffer
            for (unsigned i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    /** @brief Move constructor. Transfers ownership of the memory buffer. */
    Array(Array&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    /** @brief Destructor. Releases allocated memory buffer. */
    ~Array() {
        delete[] data;
    }

    // ---------------------------------------------------------------------
    // Assignment
    // ---------------------------------------------------------------------

    /** @brief Copy assignment operator. Performs a deep copy. */
    Array& operator=(const Array& other) {
        if (this == &other) return *this;

        // Reuse existing storage if it is sufficient
        if (capacity >= other.size) {
            size = other.size;
            for (unsigned i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
            return *this;
        }

        Complex* new_data = nullptr;
        if (other.capacity > 0) {
            new_data = new Complex[other.capacity]{};
            for (unsigned i = 0; i < other.size; ++i) {
                new_data[i] = other.data[i];
            }
        }

        delete[] data;
        data = new_data;
        size = other.size;
        capacity = other.capacity;
        return *this;
    }

    /** @brief Move assignment operator. Transfers ownership of resources. */
    Array& operator=(Array&& other) noexcept {
        if (this == &other) return *this;

        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    // ---------------------------------------------------------------------
    // Element Access
    // ---------------------------------------------------------------------

    Complex& operator[](unsigned i) noexcept { return data[i]; }
    const Complex& operator[](unsigned i) const noexcept { return data[i]; }

    Complex& front() noexcept { return data[0]; }
    const Complex& front() const noexcept { return data[0]; }

    Complex& back() noexcept { return data[size - 1]; }
    const Complex& back() const noexcept { return data[size - 1]; }

    Complex* data_ptr() noexcept { return data; }
    const Complex* data_ptr() const noexcept { return data; }

    // ---------------------------------------------------------------------
    // Iterators
    // ---------------------------------------------------------------------

    Complex* begin() noexcept { return data; }
    Complex* end() noexcept { return data + size; }

    const Complex* begin() const noexcept { return data; }
    const Complex* end() const noexcept { return data + size; }

    const Complex* cbegin() const noexcept { return data; }
    const Complex* cend() const noexcept { return data + size; }

    // ---------------------------------------------------------------------
    // Capacity
    // ---------------------------------------------------------------------

    /** @brief Checks whether the array has no elements. */
    bool empty() const noexcept { return size == 0; }

    /** @brief Returns the number of currently active elements. */
    unsigned get_size() const noexcept { return size; }

    /** @brief Returns total capacity allocated for storage. */
    unsigned get_capacity() const noexcept { return capacity; }

    /**
     * @brief Reallocates storage to hold at least @p new_cap elements.
     * @param new_cap New storage capacity.
     */
    void reserve(unsigned new_cap) {
        if (new_cap <= capacity) return;

        Complex* new_data = new Complex[new_cap]{};
        for (unsigned i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        capacity = new_cap;
    }

    /**
     * @brief Reduces memory usage by shrinking capacity to fit size.
     */
    void shrink_to_fit() {
        if (size == capacity) return;

        if (size == 0) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            return;
        }

        Complex* new_data = new Complex[size];
        for (unsigned i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        capacity = size;
    }

    // ---------------------------------------------------------------------
    // Modifiers
    // ---------------------------------------------------------------------

    /**
     * @brief Resizes array to contain @p n elements.
     * @param n New size.
     * @param value Default value to fill new slots if size expands.
     */
    void resize(unsigned n, const Complex& value = Complex{}) {
        if (n <= capacity) {
            if (n > size) {
                for (unsigned i = size; i < n; ++i) {
                    data[i] = value;
                }
            }
            size = n;
            return;
        }

        // Expand buffer via reserve() to preserve capacity integrity
        reserve(n);
        for (unsigned i = size; i < n; ++i) {
            data[i] = value;
        }
        size = n;
    }

    /** @brief Sets all elements to @p value. */
    void fill(const Complex& value) noexcept {
        for (unsigned i = 0; i < size; ++i) {
            data[i] = value;
        }
    }

    /** @brief Clears size without freeing underlying capacity. */
    void clear() noexcept {
        size = 0;
    }

    /** @brief Swaps contents with @p other Array instance. */
    void swap(Array& other) noexcept {
        Complex* td = data;
        data = other.data;
        other.data = td;

        unsigned ts = size;
        size = other.size;
        other.size = ts;

        unsigned tc = capacity;
        capacity = other.capacity;
        other.capacity = tc;
    }
};

} // namespace qm
