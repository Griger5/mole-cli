#include <gtest/gtest.h>

#include "../include/molecli/tokenize.hpp"

using namespace molecli::detail;

TEST(TokenizeTests, EmptyString) {
    auto [cmd_name, arguments] = tokenize("");

    EXPECT_EQ(cmd_name, "");
    EXPECT_EQ(arguments.size(), 0);
}

TEST(TokenizeTests, CommandNameOnly) {
    auto [cmd_name, arguments] = tokenize("cmd_name");

    EXPECT_EQ(cmd_name, "cmd_name");
    EXPECT_EQ(arguments.size(), 0);
}

TEST(TokenizeTests, RegularCall) {
    auto [cmd_name, arguments] = tokenize("cmd_name arg0 arg1 arg2");

    EXPECT_EQ(cmd_name, "cmd_name");
    ASSERT_EQ(arguments.size(), 3);

    EXPECT_EQ(arguments[0], "arg0");
    EXPECT_EQ(arguments[1], "arg1");
    EXPECT_EQ(arguments[2], "arg2");
}

TEST(TokenizeTests, QuotedString) {
    auto [cmd_name, arguments] = tokenize("cmd_name \"arg0 arg1\" arg2");

    EXPECT_EQ(cmd_name, "cmd_name");
    ASSERT_EQ(arguments.size(), 2);

    EXPECT_EQ(arguments[0], "arg0 arg1");
    EXPECT_EQ(arguments[1], "arg2");
}