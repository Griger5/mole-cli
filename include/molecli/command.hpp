#ifndef MOLECLI_COMMAND_HPP
#define MOLECLI_COMMAND_HPP

#include "typing_utils.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <tuple>

namespace molecli::detail {

using Args = std::vector<void *>;
using Caster = std::function<bool(std::string &&, void *)>;

class Command final {
private:
    std::function<void(const Args &)> func;
    Args args;
    std::vector<Caster> casters;
    std::function<void(Args &)> dealloc_args;
    std::vector<std::string> type_names;

public:
    Command() = default;

    Command(std::function<void(const Args &)> &&func, Args &&args, std::vector<Caster> &&casts, std::function<void(Args &)> &&dealloc, std::vector<std::string> &&names) {
        this->func = std::move(func);
        this->args = std::move(args);
        this->casters = std::move(casts);
        this->dealloc_args = std::move(dealloc);
        this->type_names = std::move(names);
    }
};

} // molecli::detail

#endif // MOLECLI_COMMAND_HPP