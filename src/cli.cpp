#include "molecli/cli.hpp"

#ifndef NO_DEPS
#include "../external/isocline/include/isocline.h"
#endif

namespace molecli {

CLI::~CLI() {
    for (auto [name, cmd] : this->commands) {
        cmd.dealloc();
    }
}

void CLI::add_sub_cli(std::string &&cli_name, std::string &&description, std::shared_ptr<CLI> cli_ptr, bool is_private) {
    cli_ptr->is_main = false;
    this->sub_cli[cli_name] = cli_ptr;
    
    if (!is_private) {
        this->help_messages_cli[cli_name] = detail::HelpMessage{std::move(cli_name), std::move(description), {}};
    }
}

void CLI::run_loop(std::istream &i_stream, std::ostream &o_stream) {
    std::string line;
    
    #ifndef NO_DEPS
        char *temp;
        if (&i_stream == &std::cin) {
            if (this->is_main) {
                ic_set_history(NULL, -1);
            }
        }
    #endif

    while (true) {
        #ifndef NO_DEPS
            if (&i_stream == &std::cin) {
                temp = ic_readline(this->prompt.c_str());
                line = std::string{temp};

                ic_history_add(temp);
                free(temp);
            }
            else {
                std::getline(i_stream, line, '\n');
            }
        #else
            if (&i_stream == &std::cin) {
                o_stream << this->prompt << "> ";
            }
            std::getline(i_stream, line, '\n');
        #endif

        auto [command_name, arguments] = detail::tokenize(std::move(line));

        if (command_name == "") {
            continue;
        }
        else if (this->commands.find(command_name) != this->commands.end()) {
            detail::Command::Status status = this->commands[command_name].load_arguments(std::move(arguments));

            switch(status.code) {
                case detail::Command::NO_ERROR:
                    this->commands[command_name].execute();
                    break;
                case detail::Command::INSUFFICIENT_COUNT:
                    o_stream << "Warning: Insufficient number of arguments. Expected: " << status.arg_count << ", received: " << status.error_idx << '\n';
                    break;
                case detail::Command::TOO_MANY_ARGS:
                    o_stream << "Warning: Too many arguments. Expected: " << status.arg_count << ", received: " << status.error_idx << '\n';
                    break;
                case detail::Command::WRONG_TYPE:
                    o_stream << "Warning: Wrong type of argument #" << status.error_idx + 1 << ". Argument type should be: " << status.type_name << '\n';
                    break;
            }
        }
        else if (this->additional_checks(command_name, std::move(arguments), o_stream)) {
            ;
        }
        else if (this->sub_cli.find(command_name) != this->sub_cli.end()) {
            this->sub_cli[command_name]->run_loop(i_stream, o_stream);
        }
        else if (command_name == "exit" || command_name == "EXIT") {
            break;
        }
        else if (command_name == "help" || command_name == "HELP") {
            o_stream << "AVAILABLE COMMANDS:\n";

            for (auto &[name, msg] : this->help_messages) {
                o_stream << msg;
            }

            o_stream << "\033[36mhelp\033[39m()/\033[36mHELP\033[39m()\n    Lists all available commands\n--------------------\n";
            o_stream << "\033[36mexit\033[39m()/\033[36mEXIT\033[39m()\n    Exits the current CLI\n--------------------\n";

            if (!this->help_messages_cli.empty()) {
                o_stream << "AVAILABLE CLIs:\n";

                for (auto &[name, msg] : this->help_messages_cli) {
                    o_stream << msg;
                }
            }
        }
        else {
            o_stream << "Unknown command. Maybe try using \"help\"?\n";
        }
    }
}

} // molecli