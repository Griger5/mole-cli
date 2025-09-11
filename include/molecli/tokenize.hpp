#ifndef MOLECLI_TOKENIZE_HPP
#define MOLECLI_TOKENIZE_HPP

#include <tuple>
#include <string>
#include <sstream>
#include <iostream>

namespace molecli::detail {

std::tuple<std::string, std::vector<std::string>> tokenize(std::string &&line) {
    std::vector<std::string> arguments;
    std::string command_name;
    std::string temp;

    std::stringstream str_stream;
    str_stream << line;

    std::getline(str_stream, command_name, ' ');

    while (std::getline(str_stream, temp, ' ')) {
        if (temp == "") {
            break;
        }
        arguments.push_back(temp);
    }

    return std::make_tuple(command_name, arguments);
}

} // molecli::detail

#endif // MOLECLI_TOKENIZE_HPP