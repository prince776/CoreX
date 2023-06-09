#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>
#include <type_traits>

TEST(TestReference, BasicTests) {
    {
        int x   = 5;
        int& xr = x;

        Ref<int> xRef(xr);
        xRef.get() = 100;

        EXPECT_EQ(x, 100);
        EXPECT_EQ(xr, 100);

        EXPECT_EQ(xRef.get(), 100);
    }
    {
        const int x = 5;
        Ref<const int> xRef(x);

        EXPECT_EQ(5, xRef.get());
        using getType = decltype(xRef.get());
        EXPECT_EQ(true, std::is_const_v<std::remove_reference_t<getType>>);
    }
}
