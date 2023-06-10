#pragma once

#include <CoreX/error.hpp>
#include <CoreX/iterator.hpp>

template <typename T>
constexpr inline void
Fill(ForwardIterator<T> begin, ForwardIterator<T> end, const T& val) noexcept {
    for (auto it = begin; it != end; it++) {
        *it = val;
    }
}

template <typename T1, typename T2>
[[nodiscard]] inline Res<> Copy(ForwardIterator<T1> inBegin,
                                ForwardIterator<T1> inEnd,
                                ForwardIterator<T2> outBegin,
                                ForwardIterator<T2> outEnd) noexcept {
    auto inIt  = inBegin;
    auto outIt = outBegin;
    for (; inIt != inEnd; inIt++, outIt++) {
        if (outIt == outEnd) {
            return Err(Error::OutOfBounds);
        }
        *outIt = *inIt;
    }

    return Res<>({});
}

template <typename T>
[[nodiscard]] inline Res<size_t> Find(ForwardIterator<T> begin,
                                      ForwardIterator<T> end,
                                      const remove_const_t<T>& val) noexcept {
    size_t pos = 0;
    for (auto it = begin; it != end; it++) {
        if (*it == val) {
            return Res<size_t>(pos);
        }
        pos++;
    }
    return Err<size_t>(Error::ElementNotFound);
}
