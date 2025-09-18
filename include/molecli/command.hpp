#ifndef MOLECLI_COMMAND_HPP
#define MOLECLI_COMMAND_HPP

#include "typing_utils.hpp"

#include <functional>
#include <vector>
#include <string>

namespace molecli::detail {

using Args = std::vector<void *>;
using Caster = std::function<bool(std::string &&, void *)>;
using DeallocFunc = std::function<void(Args &, const std::size_t &)>;

class Command {
protected:
    std::function<void(const Args &)> func;
    Args args;
    std::vector<Caster> casters;
    std::vector<DeallocFunc> dealloc_args;
    std::vector<std::string> type_names;

public:
    enum status_code {NO_ERROR = 0, INSUFFICIENT_COUNT = 1, TOO_MANY_ARGS = 2, WRONG_TYPE = 3};

    struct Status {
        status_code code;
        std::size_t error_idx;
        std::size_t arg_count;
        std::string type_name;
    };

    Command() = default;

    Command(std::function<void(const Args &)> &&func, Args &&args, std::vector<Caster> &&casts, std::vector<DeallocFunc> &&dealloc, std::vector<std::string> &&names);

    void dealloc();

    Status load_arguments(std::vector<std::string> &&tokens);

    void execute();
};

} // molecli::detail

#endif // MOLECLI_COMMAND_HPP