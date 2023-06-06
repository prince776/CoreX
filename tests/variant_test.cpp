#include "CoreX/variant.hpp"
#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>
#include <type_traits>

TEST(TestVariant, TestFindTemplateParamIndex) {
    size_t idx = FindTemplateParamIndex<int, char, int, bool, float>();
    EXPECT_EQ(1, idx);

    idx = FindTemplateParamIndex<char, int, double, char, float, char>();
    EXPECT_EQ(2, idx);
}

TEST(TestVariant, TestContainsDuplicateTemplTestVarianeateParams) {
    bool containsDup = ContainsDuplicateTemplateParam<int, char>();
    EXPECT_EQ(false, containsDup);

    containsDup = ContainsDuplicateTemplateParam<int, char, int>();
    EXPECT_EQ(true, containsDup);

    containsDup =
        ContainsDuplicateTemplateParam<float, double, int, char, int>();
    EXPECT_EQ(true, containsDup);
}

TEST(TestVariant, VariantTests) {
    {
        Variant<int, double> v1;

        bool isInt    = v1.holdsAlternative<int>();
        bool isDouble = v1.holdsAlternative<double>();

        EXPECT_EQ(true, isInt);
        EXPECT_EQ(false, isDouble);

        EXPECT_EQ(0, v1.get<int>());

        v1.get<int>() = 100;
        EXPECT_EQ(100, v1.get<int>());
    }
    {
        Variant<double, int> v2{100};

        bool isInt    = v2.holdsAlternative<int>();
        bool isDouble = v2.holdsAlternative<double>();

        EXPECT_EQ(true, isInt);
        EXPECT_EQ(false, isDouble);

        EXPECT_EQ(100, v2.get<int>());

        v2 = 100.0;

        isInt    = v2.holdsAlternative<int>();
        isDouble = v2.holdsAlternative<double>();

        EXPECT_EQ(false, isInt);
        EXPECT_EQ(true, isDouble);

        EXPECT_FLOAT_EQ(100.0, v2.get<double>());
    }
    {
        struct Temp {
            int x, y;
            Temp(int x, int y) : x(x), y(y) {
            }
        };

        Variant<float, int, Temp, char> v = Temp{1, 2};

        EXPECT_EQ(true, v.holdsAlternative<Temp>());

        auto& t = v.get<Temp>();

        EXPECT_EQ(1, t.x);
        EXPECT_EQ(2, t.y);

        v = 'h';

        EXPECT_EQ(true, v.holdsAlternative<char>());
        EXPECT_EQ(false, v.holdsAlternative<Temp>());

        EXPECT_EQ('h', v.get<char>());

        struct ExpectVariantLayout {
            Temp t;
            int activeVariant;
        };

        EXPECT_EQ(sizeof(ExpectVariantLayout), sizeof(v));
        EXPECT_EQ(std::alignment_of_v<ExpectVariantLayout>,
                  std::alignment_of_v<decltype(v)>);
    }
}
