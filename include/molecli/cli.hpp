#ifndef MOLECLI_CLI_HPP
#define MOLECLI_CLI_HPP

#include "command.hpp"

#include <map>
#include <string>

namespace molecli {

class CLI final {
private:
    std::map<std::string, detail::Command> commands;

public:
    CLI() {}

    void add_command(std::string command_name, std::string description, std::function<void(detail::BaseArgs *)> func, std::vector<detail::arg_type> type_vec);
};

} // molecli

#endif // MOLECLI_CLI_HPP