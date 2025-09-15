#include <gtest/gtest.h>

#include "../include/molecli/command_s.hpp"
#include "../include/molecli/caster.hpp"
#include "../include/molecli/static_vars.hpp"

using namespace molecli::detail;

class CommandStaticVariablesTests : public ::testing::Test {
protected:
    std::shared_ptr<StaticVars<int, std::string>> static_vars;

    CommandStaticVariablesTests() {
        static_vars = std::make_shared<StaticVars<int, std::string>>();
        static_vars->get<0>() = 0;
        static_vars->get<1>() = "";
    }
};

TEST_F(CommandStaticVariablesTests, DirectAccess) {
    Command_s<int, std::string> cmd{
        [](std::shared_ptr<StaticVars<int, std::string>> static_vars, const std::vector<void *> &) {
            static_vars->get<0>() = 1;
            static_vars->get<1>() = "test";
        },
        {},
        {},
        {},
        {}
    };

    cmd.execute(static_vars);

    EXPECT_EQ(static_vars->get<0>(), 1);
    EXPECT_EQ(static_vars->get<1>(), "test");
}

TEST_F(CommandStaticVariablesTests, CreateCopy) {
    Command_s<int, std::string> cmd{
        [](std::shared_ptr<StaticVars<int, std::string>> static_vars, const std::vector<void *> &) {
            int a = static_vars->get<0>();
            std::string b = static_vars->get<1>();

            a = 1;
            b = "test";
        },
        {},
        {},
        {},
        {}
    };

    cmd.execute(static_vars);

    EXPECT_EQ(static_vars->get<0>(), 0);
    EXPECT_EQ(static_vars->get<1>(), "");
}

TEST_F(CommandStaticVariablesTests, CreateReference) {
    Command_s<int, std::string> cmd{
        [](std::shared_ptr<StaticVars<int, std::string>> static_vars, const std::vector<void *> &) {
            int &a = static_vars->get<0>();
            std::string &b = static_vars->get<1>();

            a = 1;
            b = "test";
        },
        {},
        {},
        {},
        {}
    };

    cmd.execute(static_vars);

    EXPECT_EQ(static_vars->get<0>(), 1);
    EXPECT_EQ(static_vars->get<1>(), "test");
}

struct IntWrapper {
    int value;
};

TEST(CommandStaticVariablesTest, CustomType) {
    Command_s<IntWrapper, IntWrapper, IntWrapper> cmd{
        [](std::shared_ptr<StaticVars<IntWrapper, IntWrapper, IntWrapper>> static_vars, const std::vector<void *> &) {
            IntWrapper a = static_vars->get<1>();
            IntWrapper &b = static_vars->get<2>();
            
            static_vars->get<0>().value = 1;
            a.value = 1;
            b.value = 1;
        },
        {},
        {},
        {},
        {}
    };

    std::shared_ptr<StaticVars<IntWrapper, IntWrapper, IntWrapper>> static_vars = std::make_shared<StaticVars<IntWrapper, IntWrapper, IntWrapper>>();
    static_vars->get<0>().value = 0;
    static_vars->get<1>().value = 0;
    static_vars->get<2>().value = 0;

    cmd.execute(static_vars);

    EXPECT_EQ(static_vars->get<0>().value, 1);
    EXPECT_EQ(static_vars->get<1>().value, 0);
    EXPECT_EQ(static_vars->get<2>().value, 1);
}