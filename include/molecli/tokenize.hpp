#ifndef MOLECLI_PARSER_HPP
#define MOLECLI_PARSER_HPP

#include <tuple>
#include <string>
#include <sstream>

namespace molecli::detail {

std::tuple<std::string, std::vector<std::string>> tokenize(std::string &&line) {
    std::vector<std::string> arguments;
    std::string command_name;
    std::string temp;

    std::stringstream str_stream;
    str_stream << line;

    std::getline(str_stream, command_name, ' ');

    std::getline(str_stream, temp, ' ');

    while (!temp.empty()) {
        arguments.push_back(temp);
        std::getline(str_stream, temp, ' ');
    }

    return std::make_tuple(command_name, arguments);
}

} // molecli::detail

#endif // MOLECLI_PARSER_HPP