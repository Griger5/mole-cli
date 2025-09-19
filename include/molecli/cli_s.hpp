#ifndef MOLECLI_CLI_S_HPP
#define MOLECLI_CLI_S_HPP

#include "cli.hpp"

#ifndef NO_DEPS
// forward declarations for isocline library functions
extern "C" {

char* ic_readline(const char* prompt_text);
void ic_set_history(const char* fname, long max_entries);
void ic_history_add(const char* entry);

}
#endif

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
        type_names_vec.erase(type_names_vec.begin());
        this->static_commands[command_name] = detail::Command_s{std::move(func_wrapper), std::move(arg_vec), std::move(caster_vec), std::move(dealloc_vec), std::move(type_names_vec)};
    }

    bool additional_checks(const std::string &command_name, std::vector<std::string> &&arguments, std::ostream &o_stream) override {
        if (this->static_commands.find(command_name) != this->static_commands.end()) {
            detail::Command::Status status = this->static_commands[command_name].load_arguments(std::move(arguments));

            switch(status.code) {
                case detail::Command::NO_ERROR:
                    this->static_commands[command_name].execute(this->static_vars);
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
            return true;
        }
        else {
            return false;
        }
    }
};

} // molecli

#endif // MOLECLI_CLI_S_HPP