#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t) noexcept;
void free(void*) noexcept;

void Assert(bool);

#ifdef __cplusplus
}
#endif
