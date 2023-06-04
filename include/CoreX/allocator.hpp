#pragma once

#include <stddef.h>
#include <stdint.h>

#include <CoreX/concepts.hpp>

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
    { alloc.allocate(size) } -> same_as<Blk>;
    { alloc.deallocate(Blk{nullptr, size}) };
    { alloc.owns(Blk{nullptr, size}) } -> same_as<bool>;
};
