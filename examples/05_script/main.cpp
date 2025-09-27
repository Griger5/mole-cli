#include <molecli/molecli.hpp>

#include <memory>
#include <iostream>
#include <fstream>

void print(int a, int b) {
    std::cout << a << " " << b << '\n';
}

void print2(std::string a) {
    std::cout << a << '\n';
}

int main() {
    molecli::CLI main_cli;
    std::shared_ptr<molecli::CLI> second_cli = std::make_shared<molecli::CLI>();
    std::shared_ptr<molecli::CLI_s<int, double>> cli_with_static = std::make_shared<molecli::CLI_s<int, double>>();

    std::ifstream input_file{"examples/05_script/script.txt"};

    main_cli.add_command("print", "Prints two integers", print);
    second_cli->add_command("print2", "Prints a string", print2);

    cli_with_static->add_command_s("echo_static", "Echo static variables", [](MOLECLI_STATIC_VARS(cli_with_static)) {
        std::cout << GetStaticVar(0) << " " << GetStaticVar(1) << '\n';
    });
    cli_with_static->add_command_s("set_static", "Set static variables", [](MOLECLI_STATIC_VARS(cli_with_static), int a, double b) {
        GetStaticVar(0) = a;
        GetStaticVar(1) = b;
    });

    main_cli.add_sub_cli("second_cli", second_cli);
    main_cli.add_sub_cli("static_cli", cli_with_static);

    main_cli.run_loop(input_file);
}