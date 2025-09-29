#include "molecli/tokenize.hpp"

#include <sstream>
#include <iomanip>

namespace molecli::detail {

std::tuple<std::string, std::vector<std::string>> tokenize(std::string &&line) {
    std::vector<std::string> arguments;
    std::string command_name;
    std::string temp;

    std::stringstream ss{line};

    ss >> command_name;
    
    while (ss >> std::quoted(temp)) {
        arguments.push_back(temp);
    }

    return std::make_tuple(command_name, arguments);
}

} // molecli::detail