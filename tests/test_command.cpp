#include <gtest/gtest.h>

#include "../include/molecli/command.hpp"
#include "../include/molecli/caster.hpp"

using namespace molecli::detail;

class CommandLoadArgumentsTests : public ::testing::TestWithParam<std::tuple<std::vector<std::string>, Command::Status>> {
protected:
    Command cmd;

    CommandLoadArgumentsTests() {
        cmd = {
            [](const std::vector<void *> &){},
            {new int, new float},
            {cast<int>, cast<float>},
            {[](std::vector<void *> args, const std::size_t &idx){delete static_cast<int *>(args[idx]);}, [](std::vector<void *> args, const std::size_t &idx){delete static_cast<float *>(args[idx]);}},
            {"int", "float"}
        };
    }

    ~CommandLoadArgumentsTests() {
        cmd.dealloc();
    }
};

TEST_F(CommandLoadArgumentsTests, CorrectArguments) {
    Command::Status status = cmd.load_arguments(std::vector<std::string>{"1", "2"});

    EXPECT_EQ(status.code, Command::status_code::NO_ERROR);
}

TEST_F(CommandLoadArgumentsTests, TooFewArguments) {
    Command::Status status = cmd.load_arguments(std::vector<std::string>{"1"});

    EXPECT_EQ(status.code, Command::status_code::INSUFFICIENT_COUNT);
    EXPECT_EQ(status.error_idx, 1);
    EXPECT_EQ(status.arg_count, 2);
}

TEST_F(CommandLoadArgumentsTests, TooManyArguments) {
    Command::Status status = cmd.load_arguments(std::vector<std::string>{"1", "2", "3"});

    EXPECT_EQ(status.code, Command::status_code::TOO_MANY_ARGS);
    EXPECT_EQ(status.error_idx, 3);
    EXPECT_EQ(status.arg_count, 2);
}

TEST_F(CommandLoadArgumentsTests, WrongTypeOfArg) {
    Command::Status status = cmd.load_arguments(std::vector<std::string>{"1", "abc"});

    EXPECT_EQ(status.code, Command::status_code::WRONG_TYPE);
    EXPECT_EQ(status.error_idx, 1);
    EXPECT_EQ(status.arg_count, 2);
    EXPECT_EQ(status.type_name, "float");
}