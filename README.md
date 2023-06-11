# CoreX

CoreX is a CPP Header-Only Library, that contains, C++ utilities to be used in freestanding environment. I am making this to be used for OSDev.
Since OS can and should have memory management, it also has abstractions that use new/delete, however allocation is handled with Allocator throughout the code, so if you don't want new/delete, it wouldn't have it.

The goal is to write clean, composable abstractions that are well tested. I'm tired of debugging these unnecessary issues while working on the OS, that just makes it 10x harder.

## Usage

Include this CMake project and start using the header files.
This library requires a few stubs to be implemented with the following signatures:

```cpp
void Assert(bool) noexcept;
[[nodiscard]] void* malloc(size_t) noexcept;
void Free(void*) noexcept;
```

you can leave them empty if your environment doesn't have them.

You can plug your own error handling for irrecoverable errors by implementing
`Assert` as it's called when those happens.

### Which kind of errors are treated irrecoverable?

Irrecoverable errors, in this library are the ones where error handling can't enforce compile time usage requirements. For example, usage of a variant type without checking the type will lead to `Assert(false)` being called, or dereferencing a nullptr in the provided reference wrapper: `Ref<T>`.

Most other errors are recoverable, and hence returned in the following container:

```cpp
Result<T, Error>
```
