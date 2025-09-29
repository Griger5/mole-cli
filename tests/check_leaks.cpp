#include "../include/molecli/molecli.hpp"

#include <memory>
#include <sstream>

struct IntWrapper {
    int value;
};

std::optional<IntWrapper> string_to_intwrapper(std::string &&token) {
    if (token.find_first_not_of("0123456789") == std::string::npos) {
        return IntWrapper{std::stoi(token)};
    }
    else {
        return std::nullopt;
    }
}

MOLECLI_ADD_CASTER(IntWrapper, string_to_intwrapper);

std::shared_ptr<molecli::CLI_s<int, std::string, IntWrapper>> cli_with_static = std::make_shared<molecli::CLI_s<int, std::string, IntWrapper>>();

void echo_static_vars(MOLECLI_STATIC_VARS(cli_with_static)) {
    std::cout << GetStaticVar(0) << '\n';
    std::cout << GetStaticVar(1) << '\n';
    std::cout << GetStaticVar(2).value << '\n';
}

void set_static_vars(MOLECLI_STATIC_VARS(cli_with_static), int a, std::string b, IntWrapper c) {
    GetStaticVar(0) = a;
    GetStaticVar(1) = b;
    GetStaticVar(2) = c;
}

void echo(int a, float b, IntWrapper c) {
    std::cout << a << " " << b << " " << c.value << '\n';
}

void echo2(std::string a) {
    std::cout << a << '\n';
}

int main() {
    molecli::CLI main_cli;
    std::shared_ptr<molecli::CLI> second_cli = std::make_shared<molecli::CLI>();

    main_cli.add_command("echo", "Description", std::function{echo});
    second_cli->add_command("echo2", "Description", std::function{echo2});

    cli_with_static->add_command_s("echo_static_vars", "Description", std::function{echo_static_vars});
    cli_with_static->add_command_s("set_static_vars", "Description", std::function{set_static_vars});

    main_cli.add_sub_cli("second_cli", "", second_cli);
    main_cli.add_sub_cli("cli_with_static", "", cli_with_static);

    std::stringstream instructions;
    
    instructions << "echo 1 2\n" << "echo 1 2 a\n" << "echo 1 2 3 4\n" << "echo 1 2 3\n";
    instructions << "second_cli\n" << "echo2 text\n" << "exit\n";
    instructions << "cli_with_static\n" << "echo_static_vars\n" << "set_static_vars 1 test 5\n" << "echo_static_vars\n";
    instructions << "set_static_vars 1 test a\n" << "echo_static_vars\n" << "exit\n";
    instructions << "test\n" << "help\n" << "exit\n";

    main_cli.run_loop(instructions);
}