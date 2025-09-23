#include <gtest/gtest.h>

#include "../include/molecli/cli.hpp"

#include <string>
#include <sstream>

using namespace molecli;

class CLITests : public ::testing::Test {
protected:
    CLI cli;
    std::stringstream input;
    std::stringstream output;
};

std::string test_subject;

void set_test_subject(std::string a) {
    test_subject = a;
}

struct SetTestSubject {
    void operator()(std::string a) {
        test_subject = a;
    }
};

TEST_F(CLITests, AcceptsFunction) {
    cli.add_command("set_test", "", set_test_subject);
    
    input << "set_test Function\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "Function");
}

TEST_F(CLITests, AcceptsFunctionPointer) {
    void (*f_ptr)(std::string) = set_test_subject;
    cli.add_command("set_test", "", f_ptr);
    
    input << "set_test FunctionPointer\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "FunctionPointer");
}

TEST_F(CLITests, AcceptsStdFunctionObject) {
    std::function<void(std::string)> func = set_test_subject;
    cli.add_command("set_test", "", func);
    
    input << "set_test FunctionObject\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "FunctionObject");
}

TEST_F(CLITests, AcceptsFunctor) {
    cli.add_command("set_test", "", SetTestSubject());
    
    input << "set_test Functor\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "Functor");
}

TEST_F(CLITests, AcceptsLambda) {
    cli.add_command("set_test", "", [&](std::string a){
        test_subject = a;
    });
    
    input << "set_test Lambda\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(test_subject, "Lambda");
}

TEST_F(CLITests, CorrectHelpOutput) {
    cli.add_command("set_test", "Set a global variable", set_test_subject);

    input << "help\nexit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "AVAILABLE COMMANDS:\n"
    "\033[36mset_test\033[39m(std::string)\n    Set a global variable\n--------------------\n"
    "\033[36mhelp\033[39m()/\033[36mHELP\033[39m()\n    Lists all available commands\n--------------------\n"
    "\033[36mexit\033[39m()/\033[36mEXIT\033[39m()\n    Exists the current CLI\n--------------------\n");
}

TEST_F(CLITests, UnknownCommand) {
    input << "unknown 1 2\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Unknown command. Maybe try using \"help\"?\n");
}

TEST_F(CLITests, TooFewArguments) {
    cli.add_command("set_test", "Set a global variable", set_test_subject);

    input << "set_test\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Warning: Insufficient number of arguments. Expected: 1, received: 0\n");
}

TEST_F(CLITests, TooManyArguments) {
    cli.add_command("set_test", "Set a global variable", set_test_subject);

    input << "set_test test test2\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Warning: Too many arguments. Expected: 1, received: 2\n");
}

TEST_F(CLITests, WrongTypeOfArg) {
    cli.add_command("dummy", "Set a global variable", [](int a){});

    input << "dummy 2.5\n" << "exit\n";
    cli.run_loop(input, output);

    EXPECT_EQ(output.str(), "Warning: Wrong type of argument #1. Argument type should be: int\n");
}