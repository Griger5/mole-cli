#ifndef CLI_BRARY_CLI_HPP
#define CLI_BRARY_CLI_HPP

#include "command.hpp"

#include <map>
#include <string>

namespace talpasoft {

class CLI final {
private:
    std::map<std::string, Command> commands;

public:
    CLI() {}

    void add_command(std::string command_name, std::string description, std::function<void(detail::BaseArgs *)> func, std::vector<detail::arg_type> type_vec);
};

} // talpasoft

#endif // CLI_BRARY_CLI_HPP