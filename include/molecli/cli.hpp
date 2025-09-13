#ifndef MOLECLI_CLI_HPP
#define MOLECLI_CLI_HPP

#include "command.hpp"
#include "command_s.hpp"
#include "tokenize.hpp"
#include "caster.hpp"
#include "help_message.hpp"
#include "static_vars.hpp"

#include "external/isocline/include/isocline.h"

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <cxxabi.h>

namespace molecli {

using Args = std::vector<void *>;
using Caster = std::function<bool(std::string &&, void *)>;
using DeallocFunc = std::function<void(Args &, const std::size_t &)>;

template <typename... StaticTypes>
class CLI final {
public:
    using StaticVarsT = detail::StaticVars<StaticTypes...>;

private:
    const std::string prompt;
    std::map<std::string, detail::Command> commands;
    std::map<std::string, detail::HelpMessage> help_messages;
    std::map<std::string, detail::Command_s<StaticTypes...>> static_commands;
    std::shared_ptr<StaticVarsT> static_vars;

    template <std::size_t Idx, typename First = void, typename... Rest>
    void add_command_impl(Args &arg_vec, std::vector<Caster> &caster_vec, std::vector<DeallocFunc> &dealloc_vec, std::vector<std::string> &type_names_vec) {
        if constexpr (Idx > 0) { 
            arg_vec.push_back(new First);
            caster_vec.push_back(detail::cast<First>);
            dealloc_vec.push_back([](std::vector<void*> &v, std::size_t idx) {delete static_cast<First *>(v[idx]);});

            int status = -1;
            char *name = abi::__cxa_demangle(typeid(First).name(), 0, 0, &status);
            std::string name_str{name};
            if (name_str == "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >") {
                type_names_vec.push_back("std::string");
            }
            else {
                type_names_vec.push_back(name_str);
            }
            free(name);
            
            add_command_impl<Idx - 1, Rest...>(arg_vec, caster_vec, dealloc_vec, type_names_vec);
        }
    }

    template <std::size_t Idx, typename... ArgTypes>
    void vector_set_tuple(std::tuple<ArgTypes...> &t, const std::vector<void*> &v) {
        if constexpr (Idx != 0) {
            std::get<Idx-1>(t) = *static_cast<std::tuple_element_t<Idx-1, std::tuple<ArgTypes...>> *>(v[Idx-1]);
            vector_set_tuple<Idx-1>(t, v);
        }
    }

public:
    CLI() : prompt{""}, static_vars{std::make_shared<StaticVarsT>()} {}

    CLI(const std::string &p) : prompt{p} {}

    ~CLI() {
        for (auto [name, cmd] : this->commands) {
            cmd.dealloc();
        }

        for (auto [name, cmd] : this->static_commands) {
            cmd.dealloc();
        }
    }

    template <typename ReturnType, typename... ArgTypes>
    void add_command(std::string &&command_name, std::string &&description, std::function<ReturnType(ArgTypes...)> func) {
        Args arg_vec;
        std::vector<Caster> caster_vec;
        std::vector<DeallocFunc> dealloc_vec;
        std::vector<std::string> type_names_vec;
    
        this->add_command_impl<sizeof...(ArgTypes), ArgTypes...>(arg_vec, caster_vec, dealloc_vec, type_names_vec);

        std::function<void(const Args &)> func_wrapper = [=](const Args &v) {
            std::tuple<ArgTypes...> t;
            this->vector_set_tuple<sizeof...(ArgTypes)>(t, v);

            std::apply(func, t);
        };

        this->help_messages[command_name] = detail::HelpMessage{std::move(command_name), std::move(description), std::move(type_names_vec)};
        this->commands[command_name] = detail::Command{std::move(func_wrapper), std::move(arg_vec), std::move(caster_vec), std::move(dealloc_vec), std::move(type_names_vec)};
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

    void run_loop(std::ostream &stream = std::cout) {
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

#endif // MOLECLI_CLI_HPP