#include "CoreX/type_traits.hpp"
#include <CoreX/core.hpp>
#include <any>
#include <gtest/gtest.h>
#include <type_traits>
#include <vector>

TEST(TestTypeTraits, TestIsArray) {
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

TEST(TestTypeTraits, TestRemoveExtent) {
    auto isSame = std::is_same_v<int, remove_extent_t<int[]>>;
    EXPECT_EQ(isSame, true);

    isSame = std::is_same_v<int, remove_extent_t<int[5]>>;
    EXPECT_EQ(isSame, true);

    isSame = std::is_same_v<int, remove_extent_t<int>>;
    EXPECT_EQ(isSame, true);
}

TEST(TestTypeTraits, TestRemoveReference) {
    auto isSame = std::is_same_v<int, remove_reference_t<int&>>;
    EXPECT_EQ(isSame, true);

    isSame = std::is_same_v<int, remove_reference_t<int&&>>;
    EXPECT_EQ(isSame, true);

    isSame = std::is_same_v<int, remove_reference_t<int>>;
    EXPECT_EQ(isSame, true);
}
