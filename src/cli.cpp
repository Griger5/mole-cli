#include "molecli/cli.hpp"

#include "../external/isocline/include/isocline.h"

namespace molecli {

CLI::~CLI() {
    for (auto [name, cmd] : this->commands) {
        cmd.dealloc();
    }
}

void CLI::add_sub_cli(std::string &&cli_name, std::shared_ptr<CLI> cli_ptr) {
    cli_ptr->is_main = false;
    this->sub_cli[cli_name] = cli_ptr;
}

void CLI::run_loop(std::ostream &stream) {
    char *temp;
    
    if (this->is_main) {
        ic_set_history(NULL, -1);
    }

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

            switch(status.code) {
                case detail::Command::NO_ERROR:
                    this->commands[command_name].execute();
                    break;
                case detail::Command::INSUFFICIENT_COUNT:
                    stream << "Warning: Insufficient number of arguments. Expected: " << status.arg_count << ", received: " << status.error_idx << '\n';
                    break;
                case detail::Command::TOO_MANY_ARGS:
                    stream << "Warning: Too many arguments. Expected: " << status.arg_count << ", received: " << status.error_idx << '\n';
                    break;
                case detail::Command::WRONG_TYPE:
                    stream << "Warning: Wrong type of argument #" << status.error_idx + 1 << ". Argument type should be: " << status.type_name << '\n';
                    break;
            }
        }
        else if (this->sub_cli.find(command_name) != this->sub_cli.end()) {
            this->sub_cli[command_name]->run_loop();
        }
        else if (command_name == "exit" || command_name == "EXIT") {
            break;
        }
        else if (command_name == "help" || command_name == "HELP") {
            stream << "AVAILABLE COMMANDS:\n";

            for (auto &[name, msg] : this->help_messages) {
                stream << msg;
            }

            stream << "\033[36mhelp\033[39m()/\033[36mHELP\033[39m()\n    Lists all available commands\n--------------------\n";
            stream << "\033[36mexit\033[39m()/\033[36mEXIT\033[39m()\n    Exists the current CLI\n--------------------\n";
        }
        else {
            stream << "Unknown command. Maybe try using \"help\"?\n";
        }
    }
}

} // molecli