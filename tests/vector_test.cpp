#include "CoreX/string.hpp"
#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestVector, BasicTests) {
    {
        Vector<int> v{};
        EXPECT_EQ(0, v.size());

        auto front = v.front();
        EXPECT_EQ(Error::EmptyContainer, front.error());
    }
    {
        Vector<int> v(5, 10);

        EXPECT_EQ(5, v.size());
        for (const auto& x : v) {
            EXPECT_EQ(10, x);
        }

        Vector<int> v2(5, 10);
        EXPECT_EQ(v, v2);

        v2[1].value().get() = 12;

        EXPECT_EQ(false, v == v2);

        for (auto& x : v) {
            x = 12;
        }
        Fill(v2.begin(), v2.end(), 12);

        EXPECT_EQ(v, v2);

        v.push_back(13);
        EXPECT_EQ(v.size(), 6);
        EXPECT_EQ(13, v.back().value());

        v.resize(10);
        for (int i = 6; i < 10; i++) {
            EXPECT_EQ(0, v[i].value());
        }

        v.pop_back();
        EXPECT_EQ(9, v.size());
    }
    {
        Vector<int> v(3, 3);
        v[0].value().get() = 10;
        v.fastErase(0);
        EXPECT_EQ(v.size(), 2);
    }
    {
        Vector<int> v(5, 10);
        auto v2 = v;
        EXPECT_EQ(v, v2);

        for (const auto& x : v) {
            EXPECT_EQ(10, x);
        }
    }
    {
        Vector<std::string> a;
        for (int i = 0; i < 10; i++) {
            a.push_back("test");
        }

        EXPECT_EQ(10, a.size());
        for (auto v : a) {
            EXPECT_EQ(std::string("test"), v);
        }
    }
    {
        Vector<String<>> a;
        for (int i = 0; i < 10; i++) {
            a.push_back("test");
        }

        EXPECT_EQ(10, a.size());
        for (auto v : a) {
            EXPECT_EQ(String("test"), v);
        }
    }
}
