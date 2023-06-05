#include <CoreX/CoreX.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(TestAllocator, BasicTest) {
    class MockAllocator {
      public:
        Blk allocate(uint64_t) {
            return Blk{};
        }
        void deallocate(const Blk&) {
        }
        bool owns(const Blk&) {
            return false;
        }
    };

    auto isAllocator = Allocator<MockAllocator>;

    EXPECT_EQ(isAllocator, true);
}

TEST(TestAllocator, MallocatorTest) {
    auto isAllocator = Allocator<Mallocator>;
    EXPECT_EQ(isAllocator, true);

    Mallocator allocator{};
    auto blk = allocator.allocate(10);

    EXPECT_EQ(blk.size, 10);

    // Should fail if malloc was not called.
    free(blk.ptr);

    auto ptr = malloc(10);
    allocator.deallocate(Blk{ptr, 10});
}
