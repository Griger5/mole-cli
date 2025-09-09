#ifndef MOLECLI_COMMAND_S_HPP
#define MOLECLI_COMMAND_S_HPP

#include "command.hpp"
#include "static_vars.hpp"

namespace molecli::detail {

using Args = std::vector<void *>;

template <typename... StaticTypes>
class Command_s : public Command {
private:
    using StaticVarsT = StaticVars<StaticTypes...>;
    std::function<void(std::shared_ptr<StaticVarsT>, const Args &)> func_s;

public:
    Command_s() = default;

    Command_s(std::function<void(std::shared_ptr<StaticVarsT>, const Args &)> &&func, Args &&args, std::vector<Caster> &&casts, std::vector<DeallocFunc> &&dealloc, std::vector<std::string> &&names) {
        this->func_s = std::move(func);
        this->args = std::move(args);
        this->casters = std::move(casts);
        this->dealloc_args = std::move(dealloc);
        this->type_names = std::move(names);
    }

    void execute(std::shared_ptr<StaticVarsT> static_vars) {
        this->func_s(static_vars, this->args);
    }
};

} // molecli::detail

#endif // MOLECLI_COMMAND_S_HPP