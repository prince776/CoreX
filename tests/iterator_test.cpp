#include "CoreX/iterator.hpp"
#include <CoreX/core.hpp>
#include <gtest/gtest.h>

TEST(TestIterator, TestForwardIterator) {
    std::vector<int> testVec = {1, 2, 3, 4};

    ForwardIterator<int> it{&testVec[1]};

    EXPECT_EQ(*it, 2);

    auto prevIt = ++it;
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(prevIt == it, true);

    prevIt = it++;
    EXPECT_EQ(*prevIt, 3);
    EXPECT_EQ(*it, 4);

    EXPECT_EQ(prevIt != it, true);
    EXPECT_EQ(prevIt == it, false);
}
