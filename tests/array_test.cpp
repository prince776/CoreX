#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestArray, BasicTest) {
    Array<int, 5> arr(10);

    EXPECT_EQ(5, arr.size());

    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(10, arr[i].value());
    }

    int idx = 0;
    for (auto val : arr) {
        EXPECT_EQ(arr[idx++].value(), val);
    }
    idx = 0;
    for (auto& val : arr) {
        EXPECT_EQ(arr[idx++].value(), val);
    }
    idx = 0;
    for (const auto val : arr) {
        EXPECT_EQ(arr[idx++].value(), val);
    }
    idx = 0;
    for (const auto& val : arr) {
        EXPECT_EQ(arr[idx++].value(), val);
    }
    idx = 0;

    auto res = arr[2].then<Empty, Error>([](int& x) {
        x = 5;
        return Res<>(Empty());
    });
    EXPECT_EQ(res.hasError(), false);

    EXPECT_EQ(5, arr[2].value());

    auto res2 = arr[12];
    EXPECT_EQ(Error::OutOfBounds, res2.error());
}
