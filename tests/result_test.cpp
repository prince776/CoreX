#include "CoreX/result.hpp"
#include <CoreX/CoreX.hpp>
#include <gtest/gtest.h>

TEST(TestResult, BasicTests) {
    {
        Result<int, std::string> res(5);

        EXPECT_EQ(false, res.hasError());

        auto val = res.value();
        EXPECT_EQ(5, val);
    }
    {
        auto res = Result<int, std::string>::FromErr("dummy err");

        EXPECT_EQ(true, res.hasError());

        auto val = res.error();
        EXPECT_EQ(std::string("dummy err"), val);
    }
    {
        Result<int, int> r = 5;
        EXPECT_EQ(false, r.hasError());
        EXPECT_EQ(5, r.value());

        r = Result<int, int>::FromErr(10);
        EXPECT_EQ(true, r.hasError());
        EXPECT_EQ(10, r.error());
    }
}

TEST(TestResult, ChaininigTests) {
    {
        auto f = []() {
            return Result<int, std::string>(10);
        };

        auto res = f();

        EXPECT_EQ(false, res.hasError());
        EXPECT_EQ(10, res.value());

        auto res2 = res.then<int, std::string>(
            [](int prev) -> Result<int, std::string> {
                return prev + 10;
            });

        EXPECT_EQ(false, res2.hasError());
        EXPECT_EQ(20, res2.value());
    }
    {
        auto f = []() {
            return Result<int, std::string>::FromErr("err");
        };

        auto res = f().then<int, std::string>([](int prev) {
            return Result<int, std::string>(prev + 10);
        });

        EXPECT_EQ(true, res.hasError());
        EXPECT_EQ(std::string("err"), res.error());

        res = f().onError<int, std::string>([](std::string& err) {
            return Result<int, std::string>::FromErr(err + "2");
        });

        EXPECT_EQ(true, res.hasError());
        EXPECT_EQ(std::string("err2"), res.error());
    }
    {
        auto f = []() {
            return Result<int, int>::FromErr(13);
        };

        auto res = f().then<int, int>([](int prev) {
            return Result<int, int>(prev + 10);
        });

        EXPECT_EQ(true, res.hasError());
        EXPECT_EQ(13, res.error());

        res = f().onError<int, int>([](int& err) {
            return Result<int, int>::FromErr(err + 2);
        });

        EXPECT_EQ(true, res.hasError());
        EXPECT_EQ(15, res.error());
    }
}
