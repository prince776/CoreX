#include <cassert>
#include <stdlib.h>

void Assert(bool x) noexcept {
    return assert(x);
}

[[nodiscard]] void* Malloc(size_t size) noexcept {
    return malloc(size);
}

void Free(void* ptr) noexcept {
    return free(ptr);
}
