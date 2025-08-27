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

public:
    Command() = default;

    Command(std::function<void(const Args &)> &&func, Args &&args, std::vector<Caster> &&casts, std::function<void(Args &)> &&dealloc) {
        this->func = func;
        this->args = args;
        this->casters = casts;
        this->dealloc_args = dealloc;
    }
};

} // molecli::detail

#endif // MOLECLI_COMMAND_HPP