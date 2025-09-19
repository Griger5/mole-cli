#include <gtest/gtest.h>

#include "../include/molecli/help_message.hpp"

using namespace molecli::detail;

TEST(HelpMessageTests, CorrectOutput) {
    HelpMessage message{"Name", "Description Lorem Ipsum", std::vector<std::string>{"int", "float", "CustomType"}};
    std::stringstream output;

    output << message;

    EXPECT_EQ(output.str(), "\033[36mName\033[39m(int, float, CustomType)\n    Description Lorem Ipsum\n--------------------\n");
}