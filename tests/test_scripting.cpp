#include "../include/molecli/molecli.hpp"

#include <memory>
#include <sstream>
#include <fstream>
#include <cassert>

using namespace molecli;

std::stringstream output;

void print(int a, int b) {
    output << a << " " << b << '\n';
}

void print2(std::string a) {
    output << a << '\n';
}

std::shared_ptr<CLI_s<int, double>> cli_with_static = std::make_shared<CLI_s<int, double>>();

void echo_static_vars(MOLECLI_STATIC_VARS(cli_with_static)) {
    output << GetStaticVar(0) << " " << GetStaticVar(1) << '\n';
}

void set_static_vars(MOLECLI_STATIC_VARS(cli_with_static), int a, double b) {
    GetStaticVar(0) = a;
    GetStaticVar(1) = b;
}

int main() {
    CLI main_cli;
    std::shared_ptr<CLI> second_cli = std::make_shared<CLI>();

    std::ifstream input_file{"build/sample_script.txt"};

    main_cli.add_command("print", "Prints two integers", print);
    second_cli->add_command("print2", "Prints a string", print2);
    cli_with_static->add_command_s("echo_static", "Echo static variables", echo_static_vars);
    cli_with_static->add_command_s("set_static", "Set static variables", set_static_vars);

    main_cli.add_sub_cli("second_cli", "", second_cli);
    main_cli.add_sub_cli("static_cli", "", cli_with_static);

    main_cli.run_loop(input_file, output);
    
    assert(output.str() == "1 3\n"
    "Warning: Wrong type of argument #2. Argument type should be: int\n"
    "Warning: Insufficient number of arguments. Expected: 2, received: 1\n"
    "Unknown command. Maybe try using \"help\"?\n"
    "text\n"
    "0 0\n"
    "1 15\n");

    std::cout << "Script test passed!\n";
}