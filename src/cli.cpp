#include "molecli/cli.hpp"

namespace molecli {

void CLI::add_command(std::string &&command_name, std::string &&description, detail::Command &&cmd) {
    this->commands[command_name] = std::move(cmd);
}

} // talpasoft