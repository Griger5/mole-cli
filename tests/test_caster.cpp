#include <gtest/gtest.h>

#include "molecli/caster.hpp"

using namespace molecli::detail;

template <typename T>
class CasterCorrectOutputsTests : public ::testing::TestWithParam<std::tuple<std::string, T>> {
protected:
    void *output;

    CasterCorrectOutputsTests() {
        this->output = new T;
    }

    ~CasterCorrectOutputsTests() {
        delete static_cast<T*>(output);
    }
};

using CasterCorrectBoolTests = CasterCorrectOutputsTests<bool>;

TEST_P(CasterCorrectBoolTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<bool>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(*static_cast<bool*>(output), expected_result);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectBoolTests, 
    ::testing::Values(
        std::make_tuple("0", false),
        std::make_tuple("1", true),
        std::make_tuple("false", false),
        std::make_tuple("true", true),
        std::make_tuple("FALSE", false),
        std::make_tuple("TRUE", true)
    )
);

using CasterCorrectIntTests = CasterCorrectOutputsTests<int>;

TEST_P(CasterCorrectIntTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<int>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(*static_cast<int*>(output), expected_result);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectIntTests,
    ::testing::Values(
        std::make_tuple("0", 0),
        std::make_tuple("1", 1),
        std::make_tuple("-1", -1),
        std::make_tuple("123", 123)
    )
);

using CasterCorrectFloatTests = CasterCorrectOutputsTests<float>;

TEST_P(CasterCorrectFloatTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<float>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(*static_cast<float*>(output), expected_result);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectFloatTests,
    ::testing::Values(
        std::make_tuple("0", 0),
        std::make_tuple("1", 1),
        std::make_tuple("-1", -1),
        std::make_tuple("123", 123),
        std::make_tuple("1.23", 1.23),
        std::make_tuple("-1.23", -1.23)
    )
);

using CasterCorrectDoubleTests = CasterCorrectOutputsTests<double>;

TEST_P(CasterCorrectDoubleTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<double>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(*static_cast<double*>(output), expected_result);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectDoubleTests,
    ::testing::Values(
        std::make_tuple("0", 0),
        std::make_tuple("1", 1),
        std::make_tuple("-1", -1),
        std::make_tuple("123", 123),
        std::make_tuple("1.23", 1.23),
        std::make_tuple("-1.23", -1.23)
    )
);

using CasterCorrectCharTests = CasterCorrectOutputsTests<char>;

TEST_P(CasterCorrectCharTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<char>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(*static_cast<char*>(output), expected_result);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectCharTests,
    ::testing::Values(
        std::make_tuple("0", '0'),
        std::make_tuple("1", '1'),
        std::make_tuple("a", 'a')
    )
);

using CasterCorrectStringTests = CasterCorrectOutputsTests<std::string>;

TEST_P(CasterCorrectStringTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<std::string>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(*static_cast<std::string*>(output), expected_result);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectStringTests,
    ::testing::Values(
        std::make_tuple("0", "0"),
        std::make_tuple("1", "1"),
        std::make_tuple("abc", "abc"),
        std::make_tuple("abc123.,-!", "abc123.,-!")
    )
);