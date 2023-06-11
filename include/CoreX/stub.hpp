#pragma once

#include <stddef.h>

extern "C" {
void* malloc(size_t);
void free(void*) noexcept;
}

extern "C" {
void Assert(bool);
}
