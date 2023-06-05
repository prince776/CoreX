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
    ForwardIterator(T* ptr) : ptr(ptr) {
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

    // Comparision operators.
    bool operator==(const ForwardIterator& rhs) const {
        return ptr == rhs.ptr;
    }
    bool operator!=(const ForwardIterator& rhs) const {
        return !(*this == rhs);
    }

  private:
    T* ptr;
};
