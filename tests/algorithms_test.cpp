#include "CoreX/algorithms.hpp"
#include "test_utility.hpp"
#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestAlgorithms, TestFill) {
    Array<int, 10> arr;

    Fill(arr.begin(), arr.end(), 12);

    for (const auto& x : arr) {
        EXPECT_EQ(12, x);
    }

    Fill(arr.begin(), arr.begin() + 2, 13);
    for (int i = 0; i < 2; i++) {
        EXPECT_EQ(13, arr[i].value());
    }
}

TEST(TestAlgorithms, TestCopy) {
    Array<int, 10> a1;
    a1[0].value().get() = 10;
    a1[1].value().get() = 11;

    Array<int, 10> a2;

    auto res = Copy(a1.begin(), a1.end(), a2.begin(), a2.end());
    EXPECT_EQ(false, res.hasError());

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(a1[i].value(), a2[i].value());
    }

    Array<int, 5> a3;
    res = Copy(a1.begin(), a1.end(), a3.begin(), a3.end());
    EXPECT_EQ(Error::OutOfBounds, res.error());
}

TEST(TestAgorithms, TestFind) {
    Array<int, 5> a;
    a[0].value().get() = 0;
    a[1].value().get() = 1;
    a[2].value().get() = 2;
    a[3].value().get() = 3;
    a[4].value().get() = 4;

    auto pos = Find(a.begin(), a.end(), 2);
    EXPECT_EQ(2, pos.value());

    pos = Find(a.begin(), a.end(), 5);
    EXPECT_EQ(Error::ElementNotFound, pos.error());
}
