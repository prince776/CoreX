#pragma once

#include "CoreX/type_traits.hpp"
#include "CoreX/utility.hpp"
#include <CoreX/allocator.hpp>
#include <cassert>

template <typename T, Allocator Alloc = Mallocator>
class UniquePtr {
  public:
    // Normal constructors.
    UniquePtr() = default;
    UniquePtr(T* ptr, Alloc& alloc = {})
        : data(Blk{ptr, sizeof(T)}), allocator(alloc) {
    }

    ~UniquePtr() {
        reset();
    }

    // Delete copy constructors.
    UniquePtr(const UniquePtr&)            = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructors.
    UniquePtr(UniquePtr&& rhs) {
        reset(rhs.release());
    }
    UniquePtr& operator=(UniquePtr&& rhs) {
        reset(rhs.release());
        return *this;
    }

    // release releases the ownership of the ptr.
    [[nodiscard]] T* release() noexcept {
        auto oldPtr = (T*)data.ptr;
        data.ptr    = nullptr;
        return oldPtr;
    }

    // reset gives ownership of newPtr to this object, and deallocates
    // the prev held pointer if any.
    void reset(T* newPtr = nullptr) noexcept {
        auto oldPtr = (T*)data.ptr;
        data.ptr    = newPtr;
        if (oldPtr) {
            oldPtr->~T();
            allocator.deallocate(Blk{oldPtr, sizeof(T)});
        }
    }

    [[nodiscard]] T* get() noexcept {
        return (T*)data.ptr;
    }
    [[nodiscard]] const T* get() const noexcept {
        return (T*)data.ptr;
    }

    // Operator overloads.
    explicit operator bool() const noexcept {
        return data.ptr != nullptr;
    }

    [[nodiscard]] const T* operator->() const noexcept {
        return get();
    }
    [[nodiscard]] const T& operator*() const noexcept {
        assert(data.ptr != nullptr);
        return *get();
    }

    [[nodiscard]] T* operator->() noexcept {
        return get();
    }
    [[nodiscard]] T& operator*() noexcept {
        assert(data.ptr != nullptr);
        return *get();
    }

  private:
    Blk data;
    Alloc allocator;
};

template <typename T, typename Alloc, typename... Args>
    requires(!is_array_v<T>)
[[nodiscard]] auto makeUnique(Alloc allocator, Args&&... args) {
    Blk blk = allocator.allocate(sizeof(T));
    return UniquePtr<T, Alloc>(new (blk.ptr) T(forward<Args>(args)...),
                               allocator);
}
