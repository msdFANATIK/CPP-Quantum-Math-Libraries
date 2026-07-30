#pragma once

#include "array.hpp"

namespace qm {

/**
 * @brief High-level dynamic array of complex numbers.
 *
 * Vector is a user-friendly wrapper around the low-level Array class.
 * It provides a complete standard container interface (push_back, pop_back, etc.)
 * while leveraging Array as the storage back-end for optimal execution speed.
 *
 * Prefer Vector for general application-level data manipulation.
 * Prefer Array for performance-critical internals with minimal overhead.
 */
struct Vector {
    Array impl; ///< Underlying storage implementation

    // ---------------------------------------------------------------------
    // Constructors & Destructor
    // ---------------------------------------------------------------------

    /** @brief Constructs an empty vector. */
    Vector() noexcept = default;

    /**
     * @brief Constructs a vector with @p n default-initialized elements.
     * @param n Number of elements.
     */
    explicit Vector(unsigned n) : impl(n) {}

    /**
     * @brief Constructs a vector with @p n copies of @p value.
     * @param n Number of elements.
     * @param value Value used to initialize elements.
     */
    Vector(unsigned n, const Complex& value) : impl(n) {
        impl.fill(value);
    }

    /** @brief Copy constructor. Defaulted via compiler-generated Rule of 5. */
    Vector(const Vector& other) = default;

    /** @brief Move constructor. Defaulted via compiler-generated Rule of 5. */
    Vector(Vector&& other) noexcept = default;

    /** @brief Destructor. */
    ~Vector() = default;

    // ---------------------------------------------------------------------
    // Assignment
    // ---------------------------------------------------------------------

    /** @brief Copy assignment operator. */
    Vector& operator=(const Vector& other) = default;

    /** @brief Move assignment operator. */
    Vector& operator=(Vector&& other) noexcept = default;

    // ---------------------------------------------------------------------
    // Element Access
    // ---------------------------------------------------------------------

    /**
     * @brief Access element at index @p i without bounds checking.
     * @param i Index of element to access.
     * @return Reference to the target element.
     */
    Complex& operator[](unsigned i) noexcept { return impl[i]; }

    /** @brief Const overload for element access at index @p i. */
    const Complex& operator[](unsigned i) const noexcept { return impl[i]; }

    /** @brief Returns reference to the first element. */
    Complex& front() noexcept { return impl.front(); }
    const Complex& front() const noexcept { return impl.front(); }

    /** @brief Returns reference to the last element. */
    Complex& back() noexcept { return impl.back(); }
    const Complex& back() const noexcept { return impl.back(); }

    /**
     * @brief Returns pointer to the contiguous storage array.
     * @note Pointer invalidation occurs on capacity reallocation.
     */
    Complex* data() noexcept { return impl.data_ptr(); }
    const Complex* data() const noexcept { return impl.data_ptr(); }

    // ---------------------------------------------------------------------
    // Iterators
    // ---------------------------------------------------------------------

    Complex* begin() noexcept { return impl.begin(); }
    Complex* end() noexcept { return impl.end(); }

    const Complex* begin() const noexcept { return impl.begin(); }
    const Complex* end() const noexcept { return impl.end(); }

    const Complex* cbegin() const noexcept { return impl.cbegin(); }
    const Complex* cend() const noexcept { return impl.cend(); }

    // ---------------------------------------------------------------------
    // Capacity
    // ---------------------------------------------------------------------

    /** @brief Checks if vector container is empty. */
    bool empty() const noexcept { return impl.empty(); }

    /** @brief Returns current element count. */
    unsigned size() const noexcept { return impl.get_size(); }

    /** @brief Returns current allocated capacity limit. */
    unsigned capacity() const noexcept { return impl.get_capacity(); }

    /**
     * @brief Reserves minimum capacity space.
     * @param new_cap Target capacity allocation.
     */
    void reserve(unsigned new_cap) { impl.reserve(new_cap); }

    /**
     * @brief Shrinks capacity allocation to match current element size.
     */
    void shrink_to_fit() { impl.shrink_to_fit(); }

    // ---------------------------------------------------------------------
    // Modifiers
    // ---------------------------------------------------------------------

    /**
     * @brief Resizes container to hold @p n elements.
     * @param n New size.
     * @param value Default value for new elements.
     */
    void resize(unsigned n, const Complex& value = Complex{}) {
        impl.resize(n, value);
    }

    /**
     * @brief Appends element to the end. Uses 1.5x reallocation growth factor.
     * @param value Element value to push.
     */
    void push_back(const Complex& value) {
        const unsigned sz = impl.get_size();
        const unsigned cap = impl.get_capacity();

        if (sz >= cap) {
            const unsigned new_cap = (cap == 0) ? 4u : cap + (cap >> 1);
            impl.reserve(new_cap);
        }
        impl.resize(sz + 1, value);
    }

    /**
     * @brief Removes the last element from the vector.
     */
    void pop_back() noexcept {
        if (!impl.empty()) {
            impl.resize(impl.get_size() - 1);
        }
    }

    /** @brief Fills vector elements with uniform value. */
    void fill(const Complex& value) noexcept { impl.fill(value); }

    /** @brief Clears vector contents without releasing allocated memory. */
    void clear() noexcept { impl.clear(); }

    /** @brief Swaps internal data state with another vector. */
    void swap(Vector& other) noexcept { impl.swap(other.impl); }

    /** @brief Zeroes out all elements fast without reallocation (zero external deps). */
    void fill_zero() noexcept { impl.fill_zero(); }

    /** @brief Resets vector to zero-filled state. */
    void reset() noexcept { impl.fill_zero(); }
};

} // namespace qm
