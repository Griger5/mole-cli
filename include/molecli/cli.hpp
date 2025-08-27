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

    void add_command(std::string &&command_name, std::string &&description, detail::Command &&cmd);
};

} // molecli

#endif // MOLECLI_CLI_HPP