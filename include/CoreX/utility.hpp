#pragma once

#include <CoreX/type_traits.hpp>
#include <stddef.h>
#include <utility>

constexpr size_t npos = static_cast<size_t>(-1);

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
[[nodiscard]] constexpr T max(const T& t) noexcept {
    return t;
}

template <typename T>
[[nodiscard]] constexpr T min(const T& t) noexcept {
    return t;
}

template <typename T, typename... Ts>
[[nodiscard]] constexpr T max(const T& t, Ts&&... ts) noexcept {
    auto res = max(forward<Ts>(ts)...);
    if (t > res) {
        return t;
    }
    return res;
}

template <typename T, typename... Ts>
[[nodiscard]] constexpr T min(const T& t, Ts&&... ts) noexcept {
    auto res = min(forward<Ts>(ts)...);
    if (t < res) {
        return t;
    }
    return res;
}

template <typename T>
void swap(T& a, T& b) noexcept {
    T temp = move(a);
    a      = move(b);
    b      = move(temp);
}
