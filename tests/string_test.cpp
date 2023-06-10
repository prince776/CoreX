#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestString, BasicTests) {
    {
        String s = "test";

        EXPECT_EQ(4, s.size());
        EXPECT_EQ('t', s[0].value());
        EXPECT_EQ('e', s[1].value());
        EXPECT_EQ('s', s[2].value());
        EXPECT_EQ('t', s[3].value());
    }
    {
        String s(5, 's');

        EXPECT_EQ(5, s.size());
        for (size_t i = 0; i < 5; i++) {
            EXPECT_EQ('s', s[i].value());
        }

        EXPECT_EQ(Error::OutOfBounds, s[5].error());

        String s2 = s + ".test";

        EXPECT_EQ(String("sssss.test"), s2);
    }
    {
        String s = "a";
        s.push_back('b');

        EXPECT_EQ(String("ab"), s);

        s.pop_back();
        EXPECT_EQ(String("a"), s);
    }
}
