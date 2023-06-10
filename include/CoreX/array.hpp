#pragma once

#include <CoreX/allocator.hpp>
#include <CoreX/error.hpp>
#include <CoreX/iterator.hpp>
#include <CoreX/reference.hpp>
#include <cassert>

template <typename T, size_t _size>
class Array {
  public:
    explicit Array(const T& val = {}) {
        for (size_t i = 0; i < size(); i++) {
            data[i] = val;
        }
    }

    [[nodiscard]] size_t size() const noexcept {
        return _size;
    }

    [[nodiscard]] ForwardIterator<T> begin() noexcept {
        return &data[0];
    }
    [[nodiscard]] const ForwardIterator<const T> cbegin() const noexcept {
        return &data[0];
    }
    [[nodiscard]] const ForwardIterator<const T> begin() const noexcept {
        return &data[0];
    }

    [[nodiscard]] ForwardIterator<T> end() noexcept {
        return &data[0] + size();
    }
    [[nodiscard]] const ForwardIterator<const T> cend() const noexcept {
        return &data[0] + size();
    }
    [[nodiscard]] const ForwardIterator<const T> end() const noexcept {
        return &data[0] + size();
    }

    [[nodiscard]] Res<Ref<T>> operator[](size_t idx) noexcept {
        if (idx >= size()) {
            return Err<Ref<T>>(Error::OutOfBounds);
        }
        return Ref(data[idx]);
    }
    [[nodiscard]] Res<Ref<const T>> operator[](size_t idx) const noexcept {
        if (idx >= size()) {
            return Err<Ref<const T>>(Error::OutOfBounds);
        }
        return Ref<const T>(data[idx]);
    }

    template <size_t otherSize>
    [[nodiscard]] bool
    operator==(const Array<T, otherSize>& rhs) const noexcept {
        if (rhs.size() != size()) {
            return false;
        }
        for (size_t i = 0; i < size(); i++) {
            if (operator[](i) != rhs[i]) {
                return false;
            }
        }
        return true;
    }

    template <size_t otherSize>
    [[nodiscard]] bool
    operator!=(const Array<T, otherSize>& rhs) const noexcept {
        return !(*this == rhs);
    }

  private:
    T data[_size];
};
