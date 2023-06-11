#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t);
void free(void*);

void Assert(bool);

#ifdef __cplusplus
}
#endif
