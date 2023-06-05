#pragma once

#include <CoreX/type_traits.hpp>
#include <stddef.h>
#include <utility>

template <typename T>
[[nodiscard]] constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
[[nodiscard]] constexpr T&& forward(remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
[[nodiscard]] constexpr remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}

template <typename T>
[[nodiscard]] constexpr const T& min(const T& a, const T& b) noexcept {
    if (a < b) {
        return a;
    }
    return b;
}

template <typename T>
[[nodiscard]] constexpr const T& max(const T& a, const T& b) noexcept {
    if (a > b) {
        return a;
    }
    return b;
}

template <typename T>
void swap(T& a, T& b) noexcept {
    T temp = move(a);
    a      = move(b);
    b      = move(temp);
}
