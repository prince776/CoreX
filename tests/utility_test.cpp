#include <CoreX/CoreX.hpp>
#include <concepts>
#include <gtest/gtest.h>
#include <utility>
#include <vector>

TEST(TestUtility, TestForward) {
    struct Temp {};
    Temp a;

    auto isSame = std::same_as<decltype(std::forward<Temp>(Temp{})),
                               decltype(forward<Temp>(Temp{}))>;
    EXPECT_EQ(true, isSame);

    isSame = std::same_as<decltype(std::forward<Temp>(a)),
                          decltype(forward<Temp>(a))>;
    EXPECT_EQ(true, isSame);
}

TEST(TestUtility, TestMove) {
    struct Temp {};
    Temp a;

    auto isSame = std::same_as<decltype(std::move(a)), decltype(move(a))>;
    EXPECT_EQ(true, isSame);
}

TEST(TestUtility, TestMin) {
    EXPECT_EQ(5, min(5, 10));
    EXPECT_EQ(5, min(10, 5));
}

TEST(TestUtility, TestMax) {
    EXPECT_EQ(10, max(5, 10));
    EXPECT_EQ(10, max(10, 5));
}

TEST(TestUtility, TestSwap) {
    std::vector<int> a = {1, 2, 3};
    swap(a[0], a[1]);

    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 1);
}
