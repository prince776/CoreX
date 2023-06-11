#pragma once

#include "CoreX/type_traits.hpp"
#include "CoreX/utility.hpp"
#include <CoreX/allocator.hpp>
#include <CoreX/error.hpp>
#include <CoreX/reference.hpp>
#include <cassert>

template <typename T, Allocator Alloc = Mallocator>
class UniquePtr {
  public:
    // Normal constructors.
    UniquePtr(Alloc& alloc = GlobalAlloc<Alloc>)
        : data(Blk{}), allocator(alloc) {
    }
    UniquePtr(T* ptr, Alloc& alloc = GlobalAlloc<Alloc>)
        : data(Blk{ptr, sizeof(T)}), allocator(alloc) {
    }

    ~UniquePtr() {
        reset();
    }

    // Delete copy constructors.
    UniquePtr(const UniquePtr&)            = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructors.
    UniquePtr(UniquePtr&& rhs) : allocator(rhs.allocator) {
        reset(rhs.release());
    }
    UniquePtr& operator=(UniquePtr&& rhs) {
        reset(rhs.release());
        allocator.get() = rhs.allocator;
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
            getAllocator().deallocate(Blk{oldPtr, sizeof(T)});
        }
    }

    [[nodiscard]] Alloc& getAllocator() noexcept {
        return allocator.get();
    }

    [[nodiscard]] T* get() noexcept {
        return (T*)data.ptr;
    }
    [[nodiscard]] const T* get() const noexcept {
        return (const T*)data.ptr;
    }

    // Operator overloads.
    explicit operator bool() const noexcept {
        return data.ptr != nullptr;
    }

    [[nodiscard]] const T* operator->() const noexcept {
        return get();
    }
    [[nodiscard]] Res<Ref<const T>> operator*() const noexcept {
        if (get() == nullptr) {
            return Err<Ref<const T>>(Error::NullptrDereference);
        }
        return Ref<const T>(*get());
    }

    [[nodiscard]] T* operator->() noexcept {
        return get();
    }
    [[nodiscard]] Res<Ref<T>> operator*() noexcept {
        if (get() == nullptr) {
            return Err<Ref<T>>(Error::NullptrDereference);
        }
        return Ref<T>(*get());
    }

  private:
    Blk data;
    Ref<Alloc> allocator;
};

template <typename T, Allocator Alloc>
class UniquePtr<T[], Alloc> {
  public:
    // Normal constructors.
    UniquePtr(Alloc& alloc = GlobalAlloc<Alloc>)
        : data(Blk{}), allocator(alloc) {
    }
    UniquePtr(Blk blk, Alloc& alloc = GlobalAlloc<Alloc>)
        : data(blk), allocator(alloc) {
    }

    ~UniquePtr() {
        reset();
    }

    // Delete copy constructors.
    UniquePtr(const UniquePtr&)            = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructors.
    UniquePtr(UniquePtr&& rhs) : allocator(rhs.allocator) {
        reset(rhs.release());
    }
    UniquePtr& operator=(UniquePtr&& rhs) {
        reset(rhs.release());
        allocator.get() = rhs.allocator;
        return *this;
    }

    // release releases the ownership of the blk.
    [[nodiscard]] Blk release() noexcept {
        auto oldData = data;

        data = Blk{};
        return oldData;
    }

    // reset gives ownership of newPtr to this object, and deallocates
    // the prev held pointer if any.
    void reset(Blk newData = {}) noexcept {
        auto oldData = data;
        data         = newData;
        if (oldData.ptr) {
            int len = oldData.size / sizeof(T);
            for (int i = len - 1; i >= 0; i--) {
                ((T*)oldData.ptr + i)->~T();
            }
            getAllocator().deallocate(oldData);
        }
    }

    [[nodiscard]] Alloc& getAllocator() noexcept {
        return allocator.get();
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
    [[nodiscard]] Res<Ref<const T>> operator*() const noexcept {
        if (get() == nullptr) {
            return Err<Ref<const T>>(Error::NullptrDereference);
        }
        return Ref<const T>(*get());
    }

    [[nodiscard]] T* operator->() noexcept {
        return get();
    }
    [[nodiscard]] Res<Ref<T>> operator*() noexcept {
        if (get() == nullptr) {
            return Err<Ref<T>>(Error::NullptrDereference);
        }
        return Ref<T>(*get());
    }

    [[nodiscard]] Res<Ref<const T>> operator[](size_t index) const noexcept {
        if (index >= size()) {
            return Err<Ref<const T>>(Error::NullptrDereference);
        }
        return Ref<const T>(get()[index]);
    }
    [[nodiscard]] Res<Ref<T>> operator[](size_t index) noexcept {
        if (index >= size()) {
            return Err<Ref<T>>(Error::NullptrDereference);
        }
        return Ref<T>(get()[index]);
    }

    [[nodiscard]] size_t size() const noexcept {
        return data.size / sizeof(T);
    }

  private:
    Blk data;
    Ref<Alloc> allocator;
};

template <typename T, Allocator Alloc, typename... Args>
    requires(!is_array_v<T>)
[[nodiscard]] auto makeUnique(Alloc& allocator, Args&&... args) {
    Blk blk = allocator.allocate(sizeof(T));
    return UniquePtr<T, Alloc>(new (blk.ptr) T(Forward<Args>(args)...),
                               allocator);
}

template <typename T, Allocator Alloc>
    requires(is_array_v<T>)
auto makeUnique(Alloc& allocator, size_t num) {
    using BaseType = remove_extent_t<T>;

    Blk blk = allocator.allocate(sizeof(BaseType) * num);
    for (size_t i = 0; i < num; i++) {
        BaseType* ptr = ((BaseType*)blk.ptr) + i;
        new (ptr) BaseType();
    }

    return UniquePtr<T, Alloc>(blk, allocator);
}
