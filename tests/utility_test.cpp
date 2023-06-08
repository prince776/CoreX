#include "CoreX/utility.hpp"
#include <CoreX/CoreX.hpp>
#include <concepts>
#include <gtest/gtest.h>
#include <utility>
#include <vector>

TEST(TestUtility, TestForward) {
    struct Temp {};
    Temp a;

    auto isSame = std::same_as<decltype(std::forward<Temp>(Temp{})),
                               decltype(Forward<Temp>(Temp{}))>;
    EXPECT_EQ(true, isSame);

    isSame = std::same_as<decltype(std::forward<Temp>(a)),
                          decltype(Forward<Temp>(a))>;
    EXPECT_EQ(true, isSame);

    isSame =
        std::same_as<decltype(std::forward<int>(4)), decltype(Forward<int>(5))>;
    EXPECT_EQ(true, isSame);

    isSame = std::same_as<decltype(std::forward<std::string>("asd")),
                          decltype(Forward<std::string>("asd"))>;
    EXPECT_EQ(true, isSame);

    std::string test = "test";
    isSame           = std::same_as<decltype(std::forward<std::string>(test)),
                          decltype(Forward<std::string>(test))>;
    EXPECT_EQ(true, isSame);
}

TEST(TestUtility, TestMove) {
    struct Temp {};
    Temp a;

    auto isSame = std::same_as<decltype(std::move(a)), decltype(Move(a))>;
    EXPECT_EQ(true, isSame);
}

TEST(TestUtility, TestMin) {
    EXPECT_EQ(5, Min(5, 10));
    EXPECT_EQ(5, Min(10, 5));
    EXPECT_EQ(5, Min(10, 11, 12, 15, 5));
    EXPECT_EQ(5, Min(10, 100, 121, 5));
}

TEST(TestUtility, TestMax) {
    EXPECT_EQ(10, Max(5, 10));
    EXPECT_EQ(10, Max(10, 5));
    EXPECT_EQ(10, Max(10, 1, 2, 3, 4, 5));
    EXPECT_EQ(10, Max(10, 5, 6, 9, 5));
    EXPECT_EQ(10, Max(10, 10, 5));
}

TEST(TestUtility, TestSwap) {
    std::vector<int> a = {1, 2, 3};
    Swap(a[0], a[1]);

    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 1);
}
