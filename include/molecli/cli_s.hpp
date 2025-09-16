#ifndef MOLECLI_CLI_S_HPP
#define MOLECLI_CLI_S_HPP

#include "cli.hpp"

// forward declarations for isocline library functions
extern "C" {

char* ic_readline(const char* prompt_text);
void ic_set_history(const char* fname, long max_entries);
void ic_history_add(const char* entry);

}

namespace molecli {

using Args = std::vector<void *>;
using Caster = std::function<bool(std::string &&, void *)>;
using DeallocFunc = std::function<void(Args &, const std::size_t &)>;

template <typename... StaticTypes>
class CLI_s final : public CLI {
public:
    using StaticVarsT = detail::StaticVars<StaticTypes...>;
    using element_type = CLI_s<StaticTypes...>;

private:
    std::map<std::string, detail::Command_s<StaticTypes...>> static_commands;
    std::shared_ptr<StaticVarsT> static_vars;

public:
    CLI_s() : CLI{}, static_vars{std::make_shared<StaticVarsT>()} {}

    CLI_s(const std::string &p) : CLI{p}, static_vars{std::make_shared<StaticVarsT>()} {}

    ~CLI_s() {
        for (auto [name, cmd] : this->static_commands) {
            cmd.dealloc();
        }
    }

    template <typename ReturnType, typename... ArgTypes>
    void add_command_s(std::string &&command_name, std::string &&description, std::function<ReturnType(std::shared_ptr<StaticVarsT>, ArgTypes...)> func) {
        Args arg_vec;
        std::vector<Caster> caster_vec;
        std::vector<DeallocFunc> dealloc_vec;
        std::vector<std::string> type_names_vec;

        type_names_vec.push_back("StaticVars");
    
        this->add_command_impl<sizeof...(ArgTypes), ArgTypes...>(arg_vec, caster_vec, dealloc_vec, type_names_vec);

        std::function<void(std::shared_ptr<StaticVarsT>, const Args &)> func_wrapper = [=](std::shared_ptr<StaticVarsT> s, const Args &v) {
            std::tuple<ArgTypes...> t;

            this->vector_set_tuple<sizeof...(ArgTypes)>(t, v);

            auto t2 = std::tuple_cat(std::make_tuple(s), t);
            std::apply(func, t2);
        };

        this->help_messages[command_name] = detail::HelpMessage{std::move(command_name), std::move(description), std::move(type_names_vec)};
        this->static_commands[command_name] = detail::Command_s{std::move(func_wrapper), std::move(arg_vec), std::move(caster_vec), std::move(dealloc_vec), std::move(type_names_vec)};
    }

    void run_loop(std::ostream &stream = std::cout) override {
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
            else if (this->static_commands.find(command_name) != this->static_commands.end()) {
                detail::Command::Status status = this->static_commands[command_name].load_arguments(arguments);

                switch(status.code) {
                    case detail::Command::NO_ERROR:
                        this->static_commands[command_name].execute(this->static_vars);
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
};

} // molecli

#endif // MOLECLI_CLI_S_HPP