#pragma once

#include "CoreX/algorithms.hpp"
#include "CoreX/allocator.hpp"
#include <CoreX/vector.hpp>

[[nodiscard]] size_t Strlen(const char*) noexcept;

template <Allocator Alloc = Mallocator>
class String {
  public:
    String(Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : data(1, '\0', allocator) {
    }

    String(size_t size, Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : data(size + 1, '\0', allocator) {
    }

    String(size_t size,
           char val,
           Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : data(size + 1, val, allocator) {
        data.back().value().get() = '\0';
    }

    String(const char* str, Alloc& allocator = GlobalAlloc<Alloc>) noexcept
        : data(allocator) {
        auto len = Strlen(str);

        data.resize(len + 1);
        for (size_t i = 0; i < len; i++) {
            data[i].value().get() = str[i];
        }
        data[len].value().get() = '\0';
    }

    ~String() {
        data.~Vector();
    }

    [[nodiscard]] size_t size() const noexcept {
        return data.size() - 1;
    }

    [[nodiscard]] String& operator+=(const String& rhs) noexcept {
        data.pop_back();
        for (auto& val : rhs) {
            data.push_back(val);
        }
        data.push_back('\0');

        return *this;
    }

    [[nodiscard]] String& operator+=(char c) noexcept {
        push_back(c);

        return *this;
    }

    [[nodiscard]] String operator+(const String& rhs) const noexcept {
        String res = *this;
        res += rhs;

        return res;
    }

    void push_back(const char val) noexcept {
        data.pop_back();
        data.push_back(val);
        data.push_back('\0');
    }

    void pop_back() noexcept {
        data.pop_back();
        data.pop_back();
        data.push_back('\0');
    }

    [[nodiscard]] Res<Ref<char>> front() noexcept {
        if (size() == 0) {
            return Err<Ref<char>>(Error::EmptyContainer);
        }
        return data.front();
    }

    [[nodiscard]] Res<Ref<char>> back() noexcept {
        if (size() == 0) {
            return Err<Ref<char>>(Error::EmptyContainer);
        }
        return data[size() - 2];
    }

    [[nodiscard]] ForwardIterator<char> begin() noexcept {
        return data.begin();
    }
    [[nodiscard]] const ForwardIterator<const char> cbegin() const noexcept {
        return data.cbegin();
    }
    [[nodiscard]] const ForwardIterator<const char> begin() const noexcept {
        return data.cbegin();
    }

    [[nodiscard]] ForwardIterator<char> end() noexcept {
        return data.begin() + size();
    }
    [[nodiscard]] const ForwardIterator<const char> cend() const noexcept {
        return data.cbegin() + size();
    }
    [[nodiscard]] const ForwardIterator<const char> end() const noexcept {
        return data.cbegin() + size();
    }

    [[nodiscard]] Res<Ref<char>> operator[](size_t idx) noexcept {
        if (idx >= size()) {
            return Err<Ref<char>>(Error::OutOfBounds);
        }
        return data[idx];
    }
    [[nodiscard]] Res<Ref<const char>> operator[](size_t idx) const noexcept {
        if (idx >= size()) {
            return Err<Ref<const char>>(Error::OutOfBounds);
        }
        return data[idx];
    }

    [[nodiscard]] bool operator==(const String& other) const noexcept {
        return data == other.data;
    }

    [[nodiscard]] bool operator!=(const String& other) const noexcept {
        return !(*this == other);
    }

    [[nodiscard]] const char* c_str() const noexcept {
        return cbegin().get();
    }

    [[nodiscard]] String substr(size_t offset, size_t size) noexcept {
        if (offset > this->size()) {
            return String("", data.getAllocator());
        }
        size = Min(size, this->size() - offset);
        String res(size, data.getAllocator());
        for (size_t i = 0; i < size; i++) {
            res[i].value().get() = this->operator[](i + offset).value();
        }

        return res;
    }

    [[nodiscard]] Vector<String, Alloc> split(const char delimiter) noexcept {
        Vector<String, Alloc> res(data.getAllocator());
        size_t l = 0;
        auto r   = Find(cbegin(), cend(), delimiter);

        while (!r.hasError()) {
            // relatively shift it since Find returns relative to l.
            r.value() += l;

            res.push_back(substr(l, r.value() - l));
            l = r.value() + 1;
            r = Find(cbegin() + l, cend(), delimiter);
        }

        res.push_back(substr(l, size()));

        return res;
    }

  private:
    Vector<char, Alloc> data;
};

[[nodiscard]] inline size_t Strlen(const char* str) noexcept {
    size_t len = 0;
    while (str[len] != 0) {
        len++;
    }
    return len;
}
