#include <gtest/gtest.h>

#include "../include/molecli/cli_s.hpp"

#include <string>
#include <sstream>

using namespace molecli;

class CLI_sTests : public ::testing::Test {
protected:
    CLI_s<int, float> cli;
    std::stringstream input;
    std::stringstream output;
};

void set_static_vars(std::shared_ptr<CLI_s<int, float>::StaticVarsT> static_vars, int a, float b) {
    static_vars->get<0>() = a;
    static_vars->get<1>() = b;
}

void assert_static_vars(std::shared_ptr<CLI_s<int, float>::StaticVarsT> static_vars, int a, float b) {
    assert(static_vars->get<0>() == a);
    assert(static_vars->get<1>() == b);
}

inline std::string test_subject;

void set_test_subject(std::shared_ptr<CLI_s<int, float>::StaticVarsT>, std::string a) {
    test_subject = a;
}

struct SetTestSubject {
    void operator()(std::shared_ptr<CLI_s<int, float>::StaticVarsT>, std::string a) {
        test_subject = a;
    }
};

TEST_F(CLI_sTests, AcceptsFunction) {
    cli.add_command_s("set_test", "", set_test_subject);
    
    input << "set_test Function\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "Function");
}

TEST_F(CLI_sTests, AcceptsFunctionPointer) {
    void (*f_ptr)(std::shared_ptr<CLI_s<int, float>::StaticVarsT>, std::string) = set_test_subject;
    cli.add_command_s("set_test", "", f_ptr);
    
    input << "set_test FunctionPointer\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "FunctionPointer");
}

TEST_F(CLI_sTests, AcceptsStdFunctionObject) {
    std::function<void(std::shared_ptr<CLI_s<int, float>::StaticVarsT>, std::string)> func = set_test_subject;
    cli.add_command_s("set_test", "", func);
    
    input << "set_test FunctionObject\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "FunctionObject");
}

TEST_F(CLI_sTests, AcceptsFunctor) {
    cli.add_command_s("set_test", "", SetTestSubject());
    
    input << "set_test Functor\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "Functor");
}

TEST_F(CLI_sTests, AcceptsLambda) {
    cli.add_command_s("set_test", "", [&](std::shared_ptr<CLI_s<int, float>::StaticVarsT>, std::string a){
        test_subject = a;
    });
    
    input << "set_test Lambda\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "Lambda");
}

TEST_F(CLI_sTests, CommandsWork) {
    cli.add_command_s("set_static", "Set static variables", set_static_vars);
    cli.add_command_s("assert_static", "Assert values of static variables", assert_static_vars);
    
    input << "set_static 1 3.5\n" << "assert_static 1 3.5\n" << "exit\n";
    cli.run_loop(input, output);
}

TEST_F(CLI_sTests, CorrectHelpOutput) {
    cli.add_command_s("set_static", "Set static variables", set_static_vars);
    cli.add_command_s("assert_static", "Assert values of static variables", assert_static_vars);

    input << "help\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "AVAILABLE COMMANDS:\n"
    "\033[36massert_static\033[39m(StaticVars, int, float)\n    Assert values of static variables\n--------------------\n"
    "\033[36mset_static\033[39m(StaticVars, int, float)\n    Set static variables\n--------------------\n"
    "\033[36mhelp\033[39m()/\033[36mHELP\033[39m()\n    Lists all available commands\n--------------------\n"
    "\033[36mexit\033[39m()/\033[36mEXIT\033[39m()\n    Exits the current CLI\n--------------------\n");
}

TEST_F(CLI_sTests, UnknownCommand) {
    input << "unknown 1 2\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Unknown command. Maybe try using \"help\"?\n");
}

TEST_F(CLI_sTests, TooFewArguments) {
    cli.add_command_s("set_test", "Set a global variable", set_test_subject);

    input << "set_test\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Warning: Insufficient number of arguments. Expected: 1, received: 0\n");
}

TEST_F(CLI_sTests, TooManyArguments) {
    cli.add_command_s("set_test", "Set a global variable", set_test_subject);

    input << "set_test test test2\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Warning: Too many arguments. Expected: 1, received: 2\n");
}

TEST_F(CLI_sTests, WrongTypeOfArg) {
    cli.add_command_s("set_static", "Set static variables", set_static_vars);

    input << "set_static 2.5 2.5\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Warning: Wrong type of argument #1. Argument type should be: int\n");
}