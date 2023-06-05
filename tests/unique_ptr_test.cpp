#include "CoreX/allocator.hpp"
#include "CoreX/unique_ptr.hpp"
#include <CoreX/CoreX.hpp>
#include <cstdlib>
#include <dlfcn.h>
#include <gtest/gtest.h>
#include <map>
#include <memory>

std::map<uint64_t, uint64_t> memTracker;

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

  private:
    Mallocator mallocator{};
};

TEST(TestUniquePtr, BasicTests) {

    uint64_t allocatedAddr{};
    {
        int val  = 6;
        auto ptr = makeUnique<int>(TrackedMallocator{}, val);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(memTracker[allocatedAddr], sizeof(int));

        EXPECT_EQ(val, *ptr);
        val  = 10;
        *ptr = val;
        EXPECT_EQ(val, *ptr);
    }

    // Check for free.
    EXPECT_EQ(false, memTracker.contains(allocatedAddr));

    {
        struct Temp {
            int x, y;
            Temp(int x, int y) : x(x), y(y) {
            }
        };
        auto ptr = makeUnique<Temp>(TrackedMallocator{}, 4, 5);

        // Check for allocation.
        allocatedAddr = (uint64_t)ptr.get();
        EXPECT_EQ(memTracker[allocatedAddr], sizeof(Temp));

        EXPECT_EQ(4, ptr->x);
        EXPECT_EQ(5, ptr->y);

        ptr->x = 10;
        ptr->y = 11;

        EXPECT_EQ(10, ptr->x);
        EXPECT_EQ(11, ptr->y);
    }

    // Check for free.
    EXPECT_EQ(false, memTracker.contains(allocatedAddr));
}
