#ifndef MOLECLI_CLI_HPP
#define MOLECLI_CLI_HPP

#include "command.hpp"
#include "command_s.hpp"
#include "tokenize.hpp"
#include "caster.hpp"
#include "help_message.hpp"
#include "static_vars.hpp"

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>

#ifndef _MSC_VER
#include <cxxabi.h>
#endif

template <typename... Ts>
class CLI_s;

namespace molecli {

using Args = std::vector<void *>;
using Caster = std::function<bool(std::string &&, void *)>;
using DeallocFunc = std::function<void(Args &, const std::size_t &)>;

class CLI {
protected:
    const std::string prompt;
    std::map<std::string, detail::Command> commands;
    std::map<std::string, detail::HelpMessage> help_messages;
    std::map<std::string, std::shared_ptr<CLI>> sub_cli;
    bool is_main = true;

    template <std::size_t Idx, typename First = void, typename... Rest>
    void add_command_impl(Args &arg_vec, std::vector<Caster> &caster_vec, std::vector<DeallocFunc> &dealloc_vec, std::vector<std::string> &type_names_vec) {
        if constexpr (Idx > 0) { 
            arg_vec.push_back(new First);
            caster_vec.push_back(detail::cast<First>);
            dealloc_vec.push_back([](std::vector<void*> &v, std::size_t idx) {delete static_cast<First *>(v[idx]);});

            #ifndef _MSC_VER
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
            #else
                std::string name_str = typeid(First).name();
                if (name_str == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
                    type_names_vec.push_back("std::string");
                }
                else {
                    type_names_vec.push_back(name_str);
                }
            #endif

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

    virtual bool additional_checks(const std::string &command_name, std::vector<std::string> &&arguments, std::ostream &ostream) {return false;};

public:
    CLI() : prompt{""} {}

    CLI(const std::string &p) : prompt{p} {}

    virtual ~CLI();

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

    void add_sub_cli(std::string &&cli_name, std::shared_ptr<CLI> cli_ptr);

    virtual void run_loop(std::istream &i_stream = std::cin, std::ostream &o_stream = std::cout);
};

} // molecli

#endif // MOLECLI_CLI_HPP