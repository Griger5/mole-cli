#ifndef MOLECLI_CLI_HPP
#define MOLECLI_CLI_HPP

#include "command.hpp"
#include "tokenize.hpp"
#include "caster.hpp"

#include "external/isocline/src/isocline.c"

#include <map>
#include <string>
#include <vector>
#include <cxxabi.h>

#define MACRO_STRINGIFY(x) #x

namespace molecli {

using Args = std::vector<void *>;
using Caster = std::function<bool(std::string &&, void *)>;
using DeallocFunc = std::function<void(Args &, const std::size_t &)>;

class CLI final {
private:
    const std::string prompt;
    std::map<std::string, detail::Command> commands;

    template <std::size_t Idx, typename First = void, typename... Rest>
    void add_command_impl(Args &arg_vec, std::vector<Caster> &caster_vec, std::vector<DeallocFunc> &dealloc_vec, std::vector<std::string> &type_names_vec) {
        if constexpr (Idx > 0) { 
            arg_vec.push_back(new First);
            caster_vec.push_back(detail::cast<First>);
            dealloc_vec.push_back([](std::vector<void*> &v, std::size_t idx) {delete static_cast<First *>(v[idx]);});

            int status = -1;
            char *name = abi::__cxa_demangle(typeid(First).name(), 0, 0, &status);
            type_names_vec.push_back(std::string{name});
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
    CLI() : prompt{""} {}

    CLI(const std::string &p) : prompt{p} {}

    ~CLI() {
        for (auto [name, cmd] : this->commands) {
            cmd.dealloc();
        }
    }

    template <typename ReturnType, typename... ArgTypes>
    void add_command(std::string &&command_name, std::string &&description, std::function<ReturnType(ArgTypes...)> func) {
        Args arg_vec;
        std::vector<Caster> caster_vec;
        std::vector<DeallocFunc> dealloc_vec;
        std::vector<std::string> type_names_vec;
    
        add_command_impl<sizeof...(ArgTypes), ArgTypes...>(arg_vec, caster_vec, dealloc_vec, type_names_vec);

        std::function<void(const Args &)> func_wrapper = [&](const std::vector<void*> &v) {
            std::tuple<ArgTypes...> t;
            vector_set_tuple<sizeof...(ArgTypes)>(t, v);

            std::apply(func, t);
        };

        this->commands[command_name] = detail::Command{std::move(func_wrapper), std::move(arg_vec), std::move(caster_vec), std::move(dealloc_vec), std::move(type_names_vec)};
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