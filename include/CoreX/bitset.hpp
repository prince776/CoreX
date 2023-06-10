#pragma once

#include "CoreX/error.hpp"
#include <CoreX/allocator.hpp>
#include <CoreX/vector.hpp>

// Bitset stores bits in a compact manner, and returns a BitProxy to get a bit
// by reference.
// Note that the internal representation is as follows (Not that it matters):
// [63  ... 2 1 0], [127  ... 66 65 64] ...
template <Allocator Alloc = Mallocator>
class Bitset {
  public:
    static constexpr size_t BlockSize   = 64;
    static constexpr uint64_t BlockMask = ~((uint64_t)0);

    // BitProxy: precondition, valid reference, idx and offset values.
    class BitProxy {
        Bitset& ref;
        size_t idx{}, offset{};

      public:
        BitProxy(Bitset& ref, size_t idx, size_t offset)
            : ref(ref), idx(idx), offset(offset) {
            assert(idx * BlockSize + offset <= ref.size());
        }
        [[nodiscard]] operator bool() const noexcept {
            return (ref.data[idx].value() & (1LL << offset));
        }
        void operator=(uint8_t val) noexcept {
            ref.data[idx].value() |= (1L << offset);
            if (!val) {
                ref.data[idx].value() ^= (1LL << offset);
            }
        }
    };

    Bitset() = default;

    Bitset(size_t size, Alloc& alloc = GlobalAlloc<Alloc>)
        : len((size + BlockSize - 1) / BlockSize), data(len, 0, alloc) {
    }

    [[nodiscard]] Res<BitProxy> operator[](size_t idx) noexcept {
        if (idx >= len) {
            return Err<BitProxy>(Error::OutOfBounds);
        }
        return BitProxy(*this, idx / BlockSize, idx % BlockSize);
    }

    void flip() noexcept {
        for (auto& v : data) {
            v ^= BlockMask;
        }
    }

    void flip(size_t idx) noexcept {
        if (idx >= len) {
            return;
        }
        size_t offset = idx % BlockSize;
        data[idx / BlockSize] ^= (1LL << offset);
    }

    [[nodiscard]] size_t size() const noexcept {
        return len;
    }

  private:
    size_t len;
    Vector<uint64_t, Alloc> data;
};
