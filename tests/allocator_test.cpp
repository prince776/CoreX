#include <CoreX/core.hpp>
#include <gtest/gtest.h>

TEST(TestAllocator, BasicTest) {
    class MockAllocator {
      public:
        Blk allocate(uint64_t) { return Blk{}; }
        void deallocate(const Blk&) {}
        bool owns(const Blk&) { return false; }
    };

    auto isAllocator = Allocator<MockAllocator>;

    EXPECT_EQ(isAllocator, true);
}
