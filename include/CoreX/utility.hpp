#pragma once

#include <CoreX/type_traits.hpp>
#include <stddef.h>
#include <utility>

constexpr size_t npos = static_cast<size_t>(-1);

template <typename T>
[[nodiscard]] inline constexpr T&& Forward(remove_reference_t<T>&& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
[[nodiscard]] inline constexpr T&& Forward(remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
[[nodiscard]] inline constexpr remove_reference_t<T>&& Move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}

template <typename T>
[[nodiscard]] inline constexpr T Max(const T& t) noexcept {
    return t;
}

template <typename T>
[[nodiscard]] inline constexpr T Min(const T& t) noexcept {
    return t;
}

template <typename T, typename... Ts>
[[nodiscard]] inline constexpr T Max(const T& t, Ts&&... ts) noexcept {
    auto res = Max(Forward<Ts>(ts)...);
    if (t > res) {
        return t;
    }
    return res;
}

template <typename T, typename... Ts>
[[nodiscard]] inline constexpr T Min(const T& t, Ts&&... ts) noexcept {
    auto res = Min(Forward<Ts>(ts)...);
    if (t < res) {
        return t;
    }
    return res;
}

template <typename T>
inline void Swap(T& a, T& b) noexcept {
    T temp = Move(a);
    a      = Move(b);
    b      = Move(temp);
}
