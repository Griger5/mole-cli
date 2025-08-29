#ifndef MOLECLI_CLI_HPP
#define MOLECLI_CLI_HPP

#include "command.hpp"

#include "external/isocline/src/isocline.c"

#include <map>
#include <string>

namespace molecli {

class CLI final {
private:
    std::map<std::string, detail::Command> commands;

public:
    CLI() = default;

    void add_command(std::string &&command_name, std::string &&description, detail::Command &&cmd) {
        this->commands[command_name] = std::move(cmd);
    }
};

} // molecli

#endif // MOLECLI_CLI_HPP