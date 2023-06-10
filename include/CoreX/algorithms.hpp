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

template <typename T>
[[nodiscard]] inline Res<> Copy(ForwardIterator<T> inBegin,
                                ForwardIterator<T> inEnd,
                                ForwardIterator<T> outBegin,
                                ForwardIterator<T> outEnd) noexcept {
    for (auto inIt = inBegin, outIt = outBegin; inIt != inEnd;
         inIt++, outIt++) {
        if (outIt == outEnd) {
            return Err(Error::OutOfBounds);
        }
        *outIt = *inIt;
    }

    return Res<>({});
}

template <typename T>
[[nodiscard]] inline Res<size_t>
Find(ForwardIterator<T> begin, ForwardIterator<T> end, const T& val) noexcept {
    size_t pos = 0;
    for (auto it = begin; it != end; it++) {
        if (*it == val) {
            return Res<size_t>(pos);
        }
        pos++;
    }
    return Err<size_t>(Error::ElementNotFound);
}
