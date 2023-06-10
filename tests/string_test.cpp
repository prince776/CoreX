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
    {
        String s  = "abcdef";
        String s2 = s.substr(0, 3);

        EXPECT_EQ(String("abc"), s2);

        String s3 = s.substr(3, 3);
        EXPECT_EQ(String("def"), s3);

        String s4 = s.substr(4, 3);
        EXPECT_EQ(String("ef"), s4);

        String s5 = s.substr(10, 5);
        EXPECT_EQ(String(""), s5);
    }
    {
        String s      = "a-b-c";
        auto elements = s.split('-');

        EXPECT_EQ(3, elements.size());
        EXPECT_EQ(String("a"), elements[0].value());
        EXPECT_EQ(String("b"), elements[1].value());
        EXPECT_EQ(String("c"), elements[2].value());
    }
    // {
    //     String s      = "abc";
    //     auto elements = s.split('-');

    //     EXPECT_EQ(1, elements.size());
    //     EXPECT_EQ(String("abc"), elements[0].value());
    // }
    // {
    //     String s      = "";
    //     auto elements = s.split('-');

    //     EXPECT_EQ(1, elements.size());
    //     EXPECT_EQ(String(""), elements[0].value());
    // }
    // {
    //     String s      = "-";
    //     auto elements = s.split('-');

    //     EXPECT_EQ(2, elements.size());
    //     EXPECT_EQ(String(""), elements[0].value());
    //     EXPECT_EQ(String(""), elements[1].value());
    // }
}
