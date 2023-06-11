#pragma once

#include <stddef.h>
#include <stdint.h>

#include <CoreX/concepts.hpp>
#include <CoreX/stub.hpp>

struct Blk {
    void* ptr{nullptr};
    uint64_t size{0};
};

/**
 * @brief Based on cppcon talk from Andrei Alexandrescu
 * Ref: https://www.youtube.com/watch?v=LIb3L4vKZ7U
 */
template <typename Alloc>
concept Allocator = requires(Alloc alloc, uint64_t size) {
    { Alloc() };
    { alloc.allocate(size) } -> same_as<Blk>;
    { alloc.deallocate(Blk{nullptr, size}) };
    { alloc.owns(Blk{nullptr, size}) } -> same_as<bool>;
};

class Mallocator {
  public:
    Mallocator() = default;

    [[nodiscard]] Blk allocate(uint64_t size) noexcept {
        if (!size) {
            return Blk{nullptr, 0};
        }
        return Blk{malloc(size), size};
    }

    void deallocate(const Blk& blk) noexcept {
        if (blk.ptr) {
            free(blk.ptr);
        }
    }

    [[nodiscard]] bool owns(const Blk& blk) noexcept {
        return blk.ptr != nullptr;
    }
};

template <Allocator Alloc>
inline Alloc GlobalAlloc;
