#include <molecli/molecli.hpp>

#include <iostream>
#include <string>

void hello() {
    std::cout << "Hello!\n";
}

void echo(std::string str) {
    std::cout << str << '\n';
}

void add(int a, int b) {
    std::cout << a + b << '\n';
}

int main() {
    molecli::CLI cli;

    cli.add_command("hello", "Say hello to the computer", hello);
    cli.add_command("echo", "Echo a string", echo);
    cli.add_command("add", "Add two integers", add);

    cli.run_loop();
}