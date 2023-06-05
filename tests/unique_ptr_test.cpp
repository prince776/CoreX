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
        int val  = 6;
        auto ptr = makeUnique<int>(TrackedMallocator{}, val);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(int));

        EXPECT_EQ(val, *ptr);
        val  = 10;
        *ptr = val;
        EXPECT_EQ(val, *ptr);

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
        auto ptr = makeUnique<Temp>(TrackedMallocator{}, 4, 5);

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
        int num  = 6;
        auto ptr = makeUnique<int[]>(TrackedMallocator{}, num);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(int) * num);

        EXPECT_EQ(0, *ptr);
        *ptr   = 1;
        ptr[1] = 2;

        EXPECT_EQ(1, ptr[0]);
        EXPECT_EQ(2, ptr[1]);

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

        int num  = 6;
        auto ptr = makeUnique<Temp[]>(TrackedMallocator{}, num);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(ptr.allocator.memTracker[allocatedAddr], sizeof(Temp) * num);

        for (int i = 0; i < 5; i++) {
            EXPECT_EQ(4, ptr[i].x);
            EXPECT_EQ(5, ptr[i].y);
        }

        ptr[1].x = 10;
        ptr[2].y = 12;

        EXPECT_EQ(10, ptr[1].x);
        EXPECT_EQ(12, ptr[2].y);

        // Check for free.
        ptr.~UniquePtr();
        EXPECT_EQ(false, ptr.allocator.memTracker.contains(allocatedAddr));
    }
}
