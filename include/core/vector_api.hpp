#ifndef QM_VECTOR_HPP
#define QM_VECTOR_HPP

#include "array.hpp"

namespace qm {

/**
 * @file vector_api.hpp
 * @brief High-level dynamic vector abstraction built over zero-dependency aligned Array storage.
 */

/**
 * @brief High-level dynamic container wrapper for complex number sequences.
 * 
 * Provides standard container semantics (push_back, pop_back, resize) over low-level Array.
 * Designed with 64-byte alignment, zero C++ standard library dependencies,
 * and high-performance quantum simulation interfaces.
 */
struct Vector {
    Array impl; ///< Underlying 64-byte aligned contiguous storage memory.

    // ---------------------------------------------------------------------
    // Constructors & Destructor
    // ---------------------------------------------------------------------

    /** 
     * @brief Constructs an empty vector with zero size and capacity.
     */
    Vector() noexcept = default;

    /**
     * @brief Constructs a vector with @p n zero-initialized elements (0.0, 0.0).
     * @param n Initial element count.
     */
    explicit Vector(size_t n) : impl(n) {}

    /**
     * @brief Constructs a vector with @p n copies of @p value.
     * @param n Initial element count.
     * @param value Complex value used to initialize elements.
     */
    Vector(size_t n, const Complex& value) : impl() {
        impl.resize(n, value);
    }

    /** @brief Copy constructor. Performs deep copy of memory buffer. */
    Vector(const Vector& other) = default;

    /** @brief Move constructor. Transfers buffer ownership. */
    Vector(Vector&& other) noexcept = default;

    /** @brief Destructor. Automatically cleans up heap resources. */
    ~Vector() noexcept = default;

    // ---------------------------------------------------------------------
    // Assignment Operators
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
     * @param i Index of target element.
     * @return Reference to element.
     */
    Complex& operator[](size_t i) noexcept { return impl[i]; }

    /** @brief Const overload for indexing. */
    const Complex& operator[](size_t i) const noexcept { return impl[i]; }

    /** @brief Reference to first element. */
    Complex& front() noexcept { return impl.front(); }
    const Complex& front() const noexcept { return impl.front(); }

    /** @brief Reference to last active element. */
    Complex& back() noexcept { return impl.back(); }
    const Complex& back() const noexcept { return impl.back(); }

    /**
     * @brief Raw pointer to contiguous 64-byte aligned storage array.
     */
    Complex* data() noexcept { return impl.data(); }
    const Complex* data() const noexcept { return impl.data(); }

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

    /** @brief True if element count is 0. */
    bool empty() const noexcept { return impl.empty(); }

    /** @brief Active number of complex elements. */
    size_t size() const noexcept { return impl.size(); }

    /** @brief Maximum elements before buffer reallocation triggers. */
    size_t capacity() const noexcept { return impl.capacity(); }

    /**
     * @brief Reserves minimum capacity space to avoid frequent reallocations.
     * @param new_cap Required target capacity.
     */
    void reserve(size_t new_cap) { impl.reserve(new_cap); }

    /**
     * @brief Reduces capacity allocation matching exact size.
     */
    void shrink_to_fit() { impl.shrink_to_fit(); }

    // ---------------------------------------------------------------------
    // Modifiers
    // ---------------------------------------------------------------------

    /**
     * @brief Resizes container to hold @p n elements.
     * @param n Target element size.
     * @param value Default value for new slots.
     */
    void resize(size_t n, const Complex& value = Complex{0.0, 0.0}) {
        impl.resize(n, value);
    }

    /**
     * @brief Appends element to the end. Uses 1.5x reallocation growth factor.
     * @param value Element value to push.
     */
    void push_back(const Complex& value) {
        const size_t sz = impl.size();
        const size_t cap = impl.capacity();

        if (sz >= cap) {
            const size_t new_cap = (cap == 0) ? 8 : cap + (cap >> 1);
            impl.reserve(new_cap);
        }
        impl.push_back(value);
    }

    /**
     * @brief Removes the last active element from vector.
     */
    void pop_back() noexcept {
        if (!impl.empty()) {
            impl.resize(impl.size() - 1);
        }
    }

    /** @brief Overwrites active elements with uniform Complex value. */
    void fill(const Complex& value) noexcept { impl.fill(value); }

    /** @brief Resets size to zero without releasing underlying allocated heap. */
    void clear() noexcept { impl.clear(); }

    /** @brief Swaps contents with another Vector instance. */
    void swap(Vector& other) noexcept { impl.swap(other.impl); }

    /** @brief Resets all active vector elements to zero (0.0, 0.0). */
    void fill_zero() noexcept { impl.fill_zero(); }

    /** @brief Alias to fill_zero(). */
    void reset() noexcept { impl.fill_zero(); }
};

} // namespace qm

#endif // QM_VECTOR_HPP

