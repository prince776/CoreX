#pragma once

#include <stddef.h>

extern "C" {
void* malloc(size_t);
void free(void*);
}

extern "C" {
void Assert(bool);
}
