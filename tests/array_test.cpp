#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestArray, BasicTest) {
    Array<int, 5> arr(10);

    EXPECT_EQ(5, arr.size());

    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(10, arr[i]);
    }

    int idx = 0;
    for (auto val : arr) {
        EXPECT_EQ(arr[idx++], val);
    }
    idx = 0;
    for (auto& val : arr) {
        EXPECT_EQ(arr[idx++], val);
    }
    idx = 0;
    for (const auto val : arr) {
        EXPECT_EQ(arr[idx++], val);
    }
    idx = 0;
    for (const auto& val : arr) {
        EXPECT_EQ(arr[idx++], val);
    }
    idx = 0;

    arr[2] = 5;
    arr[3] = 10;

    EXPECT_EQ(5, arr[2]);
    EXPECT_EQ(10, arr[3]);
}
