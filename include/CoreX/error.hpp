#pragma once

#include <CoreX/result.hpp>

enum class Error : int32_t {
    OutOfBounds,
    NullptrDereference,
    EmptyContainer,
    ElementNotFound,
};

class Empty {};

template <typename T = Empty, typename E = Error>
using Res = Result<T, E>;

template <typename T = Empty>
[[nodiscard]] inline constexpr Res<T> Err(Error err) {
    return Res<T>::FromErr(err);
}
