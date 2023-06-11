#pragma once

#include <stddef.h>

[[nodiscard]] void* Malloc(size_t) noexcept;
void Free(void*) noexcept;

void Assert(bool) noexcept;
