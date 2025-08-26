#include "molecli/cli.hpp"

namespace molecli {

void CLI::add_command(std::string command_name, std::string description, std::function<void(detail::BaseArgs *)> func, std::vector<detail::arg_type> type_vec) {
    this->commands[command_name] = Command{func, type_vec};
}

} // talpasoft