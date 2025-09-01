#ifndef MOLECLI_CLI_HPP
#define MOLECLI_CLI_HPP

#include "command.hpp"
#include "tokenize.hpp"

#include "external/isocline/src/isocline.c"

#include <map>
#include <string>

namespace molecli {

class CLI final {
private:
    const std::string prompt;
    std::map<std::string, detail::Command> commands;

public:
    CLI() : prompt{""} {}

    CLI(const std::string &p) : prompt{p} {}

    void add_command(std::string &&command_name, std::string &&description, detail::Command &&cmd) {
        this->commands[command_name] = std::move(cmd);
    }

    void run_loop() {
        char *temp;
        ic_set_history(NULL, -1);

        while (true) {
            temp = ic_readline(this->prompt.c_str());
            auto [command_name, arguments] = detail::tokenize(std::string{temp});

            ic_history_add(temp);
            free(temp);

            if (command_name == "") {
                continue;
            }
            else if (this->commands.find(command_name) != this->commands.end()) {
                detail::Command::Status status = this->commands[command_name].load_arguments(arguments);

                if (status.code == detail::Command::NO_ERROR) {
                    this->commands[command_name].execute();
                }
            }
            else if (command_name == "exit" || command_name == "EXIT") {
                break;
            }
        }
    }
};

} // molecli

#endif // MOLECLI_CLI_HPP