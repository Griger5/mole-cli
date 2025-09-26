#include <molecli/molecli.hpp>

#include <vector>
#include <string>
#include <algorithm>

int main() {
    molecli::CLI_s<std::vector<int>, std::string> cli;

    cli.add_command_s("add_number", "Add number to the collection", [](MOLECLI_STATIC_VARS(cli), int num) {
        GetStaticVar(0).push_back(num);
    });

    cli.add_command_s("show_collection", "Display all elements of the collection", [](MOLECLI_STATIC_VARS(cli)) {
        for (auto elem : GetStaticVar(0)) {
            std::cout << elem << " ";
        }
        std::cout << '\n';
    });

    cli.add_command_s("set_string", "Set the value of a string", [](MOLECLI_STATIC_VARS(cli), std::string new_value) {
        GetStaticVar(1) = new_value;
    });

    cli.add_command_s("show_string", "Display the string", [](MOLECLI_STATIC_VARS(cli)) {
        std::cout << GetStaticVar(1) << '\n';
    });

    cli.add_command_s("to_upper", "Print the string with all uppercase letters", [](MOLECLI_STATIC_VARS(cli)) {
        std::string str = GetStaticVar(1);

        std::transform(str.begin(), str.end(), str.begin(), toupper);
        std::cout << str << '\n';
    });

    cli.add_command_s("to_lower", "Print the string with all lowercase letters", [](MOLECLI_STATIC_VARS(cli)) {
        std::string str = GetStaticVar(1);

        std::transform(str.begin(), str.end(), str.begin(), tolower);
        std::cout << str << '\n';
    });

    cli.run_loop();
}