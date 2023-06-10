#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestBitset, BasicTests) {
    {
        Bitset<> s(10);

        for (size_t i = 0; i < s.size(); i++) {
            auto bit = s[i];
            EXPECT_EQ(false, bit.hasError());

            EXPECT_EQ(false, bit.value());
            bit.value() = 1;

            auto newBit = s[i];
            EXPECT_EQ(true, bit.value());
        }
    }
}
