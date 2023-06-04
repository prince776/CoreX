# CoreX

CoreX is a CPP Header-Only Library, that contains, C++ utilities to be used in freestanding environment. I am making this to be used for OSDev.
Since OS can and should have memory management, it also has abstractions that use new/delete, however allocation is handled with Allocator throughout the code, so if you don't want new/delete, it wouldn't have it.

The goal is to write clean, composable abstractions that are well tested. I'm tired of debugging these unnecessary issues while working on the OS, that just makes it 10x harder.
