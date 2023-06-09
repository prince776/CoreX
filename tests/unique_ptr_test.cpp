#include "CoreX/allocator.hpp"
#include "CoreX/unique_ptr.hpp"
#include <CoreX/CoreX.hpp>
#include <cstdlib>
#include <gtest/gtest.h>
#include <map>
#include <memory>

class TrackedMallocator {
  public:
    TrackedMallocator() = default;

    [[nodiscard]] Blk allocate(uint64_t size) noexcept {
        auto blk = mallocator.allocate(size);

        memTracker[(uint64_t)blk.ptr] = size;
        return blk;
    }

    void deallocate(const Blk& blk) noexcept {
        memTracker.erase((uint64_t)blk.ptr);
        mallocator.deallocate(blk);
    }

    [[nodiscard]] bool owns(const Blk& blk) noexcept {
        return blk.ptr != nullptr;
    }

    std::map<uint64_t, uint64_t> memTracker{};

  private:
    Mallocator mallocator{};
};

TEST(TestUniquePtr, NonArray) {
    uint64_t allocatedAddr{};
    {
        int val = 6;
        TrackedMallocator trackedMallocator;
        auto ptr = makeUnique<int>(trackedMallocator, val);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(int));

        EXPECT_EQ(val, (*ptr).value().get());
        val                  = 10;
        (*ptr).value().get() = val;
        EXPECT_EQ(val, (*ptr).value().get());

        // Check for free.
        ptr.~UniquePtr();
        EXPECT_EQ(false, ptr.allocator.memTracker.contains(allocatedAddr));
    }

    {
        struct Temp {
            int x, y;
            Temp(int x, int y) : x(x), y(y) {
            }
        };
        TrackedMallocator trackedMallocator;
        auto ptr = makeUnique<Temp>(trackedMallocator, 4, 5);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(Temp));

        EXPECT_EQ(4, ptr->x);
        EXPECT_EQ(5, ptr->y);

        ptr->x = 10;
        ptr->y = 11;

        EXPECT_EQ(10, ptr->x);
        EXPECT_EQ(11, ptr->y);

        // Check for free.
        ptr.~UniquePtr();
        EXPECT_EQ(false, ptr.allocator.memTracker.contains(allocatedAddr));
    }
}

TEST(TestUniquePtr, Array) {
    uint64_t allocatedAddr{};
    {
        int num = 6;
        TrackedMallocator trackedMallocator;
        auto ptr = makeUnique<int[]>(trackedMallocator, num);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(int) * num);

        EXPECT_EQ(0, (*ptr).value().get());
        (*ptr).value().get() = 1;
        ptr[1].value().get() = 2;

        EXPECT_EQ(1, ptr[0].value().get());
        EXPECT_EQ(2, ptr[1].value().get());

        // Check for free.
        ptr.~UniquePtr();
        EXPECT_EQ(false, ptr.allocator.memTracker.contains(allocatedAddr));
    }

    {
        struct Temp {
            int x, y;
            Temp() : x(4), y(5) {
            }
        };

        int num = 6;
        TrackedMallocator trackedMallocator;
        auto ptr = makeUnique<Temp[]>(trackedMallocator, num);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(Temp) * num);

        for (int i = 0; i < 5; i++) {
            EXPECT_EQ(4, ptr[i].value().get().x);
            EXPECT_EQ(5, ptr[i].value().get().y);
        }

        ptr[1].value().get().x = 10;
        ptr[2].value().get().y = 12;

        EXPECT_EQ(10, ptr[1].value().get().x);
        EXPECT_EQ(12, ptr[2].value().get().y);

        // Check for free.
        ptr.~UniquePtr();
        EXPECT_EQ(false, ptr.allocator.memTracker.contains(allocatedAddr));
    }
    {
        int* x = nullptr;
        Mallocator m;
        auto ptr            = UniquePtr<int>(x, m);
        auto dereferenceRes = *ptr;

        EXPECT_EQ(Error::NullptrDereference, dereferenceRes.error());
    }
    {
        Mallocator m;
        auto ptr = makeUnique<int[]>(m, 5);

        auto res = ptr[5];
        EXPECT_EQ(Error::NullptrDereference, res.error());
    }
}
