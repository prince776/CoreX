#include "CoreX/type_traits.hpp"
#include <CoreX/core.hpp>
#include <any>
#include <gtest/gtest.h>
#include <vector>

TEST(TestIsArray, BasicAssertions) {
    struct testCase {
        std::any type;
        bool expectedArray;
    };

    int x1;
    int x2[5];
    int x3[] = {1, 2, 3, 4};

    auto isArray = is_array_v<decltype(x1)>;
    EXPECT_EQ(isArray, false);

    isArray = is_array_v<decltype(x2)>;
    EXPECT_EQ(isArray, true);

    isArray = is_array_v<decltype(x3)>;
    EXPECT_EQ(isArray, true);
}
