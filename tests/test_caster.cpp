#include <gtest/gtest.h>

#include "../include/molecli/caster.hpp"

using namespace molecli::detail;

template <typename T>
class CasterCorrectOutputTests : public ::testing::TestWithParam<std::tuple<std::string, T>> {
protected:
    void *output;

    CasterCorrectOutputTests() {
        this->output = new T;
    }

    ~CasterCorrectOutputTests() {
        delete static_cast<T*>(output);
    }
};

template <typename T>
class CasterIncorrectOutputTests : public ::testing::TestWithParam<std::tuple<std::string>> {
protected:
    void *output;

    CasterIncorrectOutputTests() {
        this->output = new T;
    }

    ~CasterIncorrectOutputTests() {
        delete static_cast<T*>(output);
    }
};

using CasterCorrectBoolTests = CasterCorrectOutputTests<bool>;

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

using CasterIncorrectBoolTests = CasterIncorrectOutputTests<bool>;

TEST_P(CasterIncorrectBoolTests, CastFails) {
    auto [token] = GetParam();

    bool success = cast<bool>(std::move(token), output);

    ASSERT_FALSE(success);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterIncorrectBoolTests, 
    ::testing::Values(
        std::make_tuple(""),
        std::make_tuple("notbool")
    )
);

using CasterCorrectIntTests = CasterCorrectOutputTests<int>;

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

using CasterIncorrectIntTests = CasterIncorrectOutputTests<int>;

TEST_P(CasterIncorrectIntTests, CastFails) {
    auto [token] = GetParam();

    bool success = cast<int>(std::move(token), output);

    ASSERT_FALSE(success);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterIncorrectIntTests, 
    ::testing::Values(
        std::make_tuple(""),
        std::make_tuple("1.2"),
        std::make_tuple("a1"),
        std::make_tuple("1a"),
        std::make_tuple("notint")
    )
);

using CasterCorrectFloatTests = CasterCorrectOutputTests<float>;

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
        std::make_tuple("-1.23", -1.23),
        std::make_tuple(".5", 0.5)
    )
);

using CasterIncorrectFloatTests = CasterIncorrectOutputTests<float>;

TEST_P(CasterIncorrectFloatTests, CastFails) {
    auto [token] = GetParam();

    bool success = cast<float>(std::move(token), output);

    ASSERT_FALSE(success);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterIncorrectFloatTests, 
    ::testing::Values(
        std::make_tuple(""),
        std::make_tuple("1..2"),
        std::make_tuple("1.2."),
        std::make_tuple(".1.2"),
        std::make_tuple("a1"),
        std::make_tuple("1a"),
        std::make_tuple("notfloat")
    )
);

using CasterCorrectDoubleTests = CasterCorrectOutputTests<double>;

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
        std::make_tuple("-1.23", -1.23),
        std::make_tuple(".5", 0.5)
    )
);

using CasterIncorrectDoubleTests = CasterIncorrectOutputTests<double>;

TEST_P(CasterIncorrectDoubleTests, CastFails) {
    auto [token] = GetParam();

    bool success = cast<double>(std::move(token), output);

    ASSERT_FALSE(success);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterIncorrectDoubleTests, 
    ::testing::Values(
        std::make_tuple(""),
        std::make_tuple("1..2"),
        std::make_tuple("1.2."),
        std::make_tuple(".1.2"),
        std::make_tuple("a1"),
        std::make_tuple("1a"),
        std::make_tuple("notdouble")
    )
);

using CasterCorrectCharTests = CasterCorrectOutputTests<char>;

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

using CasterIncorrectCharTests = CasterIncorrectOutputTests<char>;

TEST_P(CasterIncorrectCharTests, CastFails) {
    auto [token] = GetParam();

    bool success = cast<char>(std::move(token), output);

    ASSERT_FALSE(success);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterIncorrectCharTests, 
    ::testing::Values(
        std::make_tuple(""),
        std::make_tuple("aa"),
        std::make_tuple("notchar")
    )
);

using CasterCorrectStringTests = CasterCorrectOutputTests<std::string>;

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

struct Empty {};

TEST(CasterTests, ThrowOnUnknownType) {
    void *output = new Empty;

    EXPECT_THROW({
        bool success = cast<Empty>("abc", output);
    }, std::logic_error);
    
    delete static_cast<Empty*>(output);
}

struct IntWrapper {
    int value;
};

template <>
bool molecli::detail::cast<IntWrapper>(std::string &&token, void *output) {
    IntWrapper *output_intwrapper = static_cast<IntWrapper*>(output);
    
    if (token.find_first_not_of("0123456789") == std::string::npos) {
        output_intwrapper->value = std::stoi(token);
        return true;
    }
    else {
        return false;
    }
}

using CasterCorrectUserDefinedTypeTests = CasterCorrectOutputTests<IntWrapper>;

TEST_P(CasterCorrectUserDefinedTypeTests, ReturnsCorrectValue) {
    auto [token, expected_result] = GetParam();

    bool success = cast<IntWrapper>(std::move(token), output);

    ASSERT_TRUE(success);
    EXPECT_EQ(static_cast<IntWrapper*>(output)->value, expected_result.value);
}

INSTANTIATE_TEST_CASE_P(
    CasterTests,
    CasterCorrectUserDefinedTypeTests,
    ::testing::Values(
        std::make_tuple("0", IntWrapper{0}),
        std::make_tuple("1", IntWrapper{1})
    )
);