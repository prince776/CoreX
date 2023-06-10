#pragma once

#include "CoreX/utility.hpp"
#include <CoreX/algorithms.hpp>
#include <CoreX/error.hpp>
#include <CoreX/iterator.hpp>
#include <CoreX/unique_ptr.hpp>
#include <cassert>

template <typename T, Allocator Alloc = Mallocator>
class Vector {
  public:
    constexpr Vector(Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : data(allocator), m_size(0), capacity(0), allocator(allocator) {
    }
    Vector(size_t size, Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : m_size(size), capacity(size), allocator(allocator) {
        data = makeUnique<T[], Alloc>(allocator, capacity);
        Fill(begin(), end(), T{});
    }
    Vector(size_t size,
           const T& val,
           Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : m_size(size), capacity(size), allocator(allocator) {
        data = makeUnique<T[], Alloc>(allocator, capacity);
        Fill(begin(), end(), val);
    }

    Vector(const Vector<T, Alloc>& v) noexcept
        : m_size(v.size()), capacity(v.capacity), allocator(v.allocator) {
        // reset before to make sure old allocator is used for deallocation.
        data.reset();

        auto newData = makeUnique<T[], Alloc>(allocator, capacity);
        data         = move(newData);

        auto res = Copy(v.begin(), v.end(), begin(), end());
        // Error handling in constructor? HOW?
        assert(!res.hasError());
    }
    [[nodiscard]] Vector& operator=(const Vector<T, Alloc>& v) noexcept {
        m_size   = v.size();
        capacity = v.capacity;

        // reset before to make sure old allocator is used for deallocation.
        data.reset();
        allocator = v.allocator;

        auto newData = makeUnique<T[], Alloc>(allocator, capacity);
        data         = move(newData);

        auto res = Copy(v.begin(), v, end(), begin(), end());
        assert(!res.hasError());
        return *this;
    }

    Vector(Vector<T, Alloc>&& v) noexcept            = default;
    Vector& operator=(Vector<T, Alloc>&& v) noexcept = default;

    ~Vector() = default;

    [[nodiscard]] size_t size() const noexcept {
        return m_size;
    }

    [[nodiscard]] ForwardIterator<T> begin() noexcept {
        return data.get();
    }
    [[nodiscard]] const ForwardIterator<const T> begin() const noexcept {
        return data.get();
    }

    [[nodiscard]] ForwardIterator<T> end() noexcept {
        return data.get() + size();
    }
    [[nodiscard]] const ForwardIterator<const T> end() const noexcept {
        return data.get() + size();
    }

    [[nodiscard]] Res<Ref<T>> operator[](size_t idx) noexcept {
        if (idx >= size()) {
            return Err<Ref<T>>(Error::OutOfBounds);
        }
        return data[idx];
    }
    [[nodiscard]] Res<Ref<const T>> operator[](size_t idx) const noexcept {
        if (idx >= size()) {
            return Err<Ref<const T>>(Error::OutOfBounds);
        }
        return data[idx];
    }

    void resize(size_t newSize) noexcept {
        auto newCapacity = capacity;
        if (newSize > newCapacity) {
            newCapacity = newSize + 1;
        }
        realloc(newSize, newCapacity);
    }

    void push_back(const T& val) noexcept {
        if (capacity == m_size) {
            realloc(m_size, m_size * 2 + 1);
        }
        data[m_size++].value().get() = val;
    }

    void pop_back() noexcept {
        if (size() > 0) {
            data[m_size].value().get().~T();
            m_size--;
        }
    }

    [[nodiscard]] Res<Ref<T>> front() noexcept {
        if (size() == 0) {
            return Err<Ref<T>>(Error::EmptyContainer);
        }
        return data[0];
    }

    [[nodiscard]] Res<Ref<T>> back() noexcept {
        if (size() == 0) {
            return Err<Ref<T>>(Error::EmptyContainer);
        }
        return data[size() - 1];
    }

    [[nodiscard]] bool operator==(const Vector& other) const noexcept {
        if (size() != other.size()) {
            return false;
        }
        auto it = begin(), it2 = other.begin();
        for (; it != end(); it++, it2++) {
            if (*it != *it2) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool operator!=(const Vector& other) const noexcept {
        return !(*this == other);
    }

    void fastErase(size_t pos) noexcept {
        if (pos > size()) {
            return;
        }
        if (pos != size() - 1) {
            Swap(data[pos].value().get(), data[size() - 1].value().get());
        }
        return pop_back();
    }

    [[nodiscard]] Alloc& getAllocator() noexcept {
        return allocator.get();
    }

  protected:
    // realloc reallocates the vector with new size and capacity. If the newSize
    // is smaller than prev size, the elements are truncated.
    void realloc(size_t newSize, size_t newCapacity) noexcept {
        newCapacity = Max(newCapacity, newSize);

        UniquePtr<T[], Alloc> newData =
            makeUnique<T[], Alloc>(allocator, newCapacity);

        size_t minSize = Min(m_size, newSize);
        for (size_t i = 0; i < minSize; i++) {
            newData[i].value().get() = Move(data[i].value());
        }

        for (size_t i = minSize; i < newSize; i++) {
            newData[i].value().get() = T{};
        }

        data     = Move(newData);
        m_size   = newSize;
        capacity = newCapacity;
    }

  private:
    UniquePtr<T[], Alloc> data;
    size_t m_size = 0, capacity = 0;
    Ref<Alloc> allocator;
};
