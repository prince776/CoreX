#include <CoreX/core.hpp>
#include <gtest/gtest.h>

TEST(TestConcepts, TestIsSame) {
#define int2 int
    auto isSame = same_as<int, int2>;
    EXPECT_EQ(isSame, true);

    isSame = same_as<int, char>;
    EXPECT_EQ(isSame, false);
}
