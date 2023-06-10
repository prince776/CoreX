#pragma once

template <typename T>
class ForwardIterator {
  public:
    // Type aliases.
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = const T&;

    // Constructor.
    constexpr ForwardIterator(T* ptr) : ptr(ptr) {
    }

    // Dereferencing.
    reference operator*() {
        return *ptr;
    }
    const_reference operator*() const {
        return *ptr;
    }
    pointer operator->() const {
        return ptr;
    }

    // Increment operators.
    ForwardIterator& operator++() {
        ptr++;
        return *this;
    }
    ForwardIterator operator++(int) {
        ForwardIterator temp{ptr};
        operator++();
        return temp;
    }

    [[nodiscard]] constexpr ForwardIterator<T> operator+(int x) const noexcept {
        return ForwardIterator<T>(ptr + x);
    }

    // Comparision operators.
    bool operator==(const ForwardIterator& rhs) const {
        return ptr == rhs.ptr;
    }
    bool operator!=(const ForwardIterator& rhs) const {
        return !(*this == rhs);
    }

    [[nodiscard]] T* get() noexcept {
        return ptr;
    }

  private:
    T* ptr;
};
