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
using DeallocFunc = std::function<void(Args &, const std::size_t &)>;

class Command final {
private:
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

    Command(std::function<void(const Args &)> &&func, Args &&args, std::vector<Caster> &&casts, std::vector<DeallocFunc> &&dealloc, std::vector<std::string> &&names) {
        this->func = std::move(func);
        this->args = std::move(args);
        this->casters = std::move(casts);
        this->dealloc_args = std::move(dealloc);
        this->type_names = std::move(names);
    }

    void dealloc() {
        for (std::size_t i = 0; i < this->args.size(); i++) {
            this->dealloc_args[i](this->args, i);
        }
    }

    Status load_arguments(std::vector<std::string> tokens) {
        std::size_t i = 0;
        bool cast_success;

        for (auto &&token : tokens) {
            if (i >= this->args.size()) {
                return Status{TOO_MANY_ARGS, i+1, this->args.size(), ""};
            }
            else if (!this->casters[i](std::move(token), this->args[i])) {
                return Status{WRONG_TYPE, i, this->args.size(), this->type_names[i]};
            }

            i++;
        }

        if (i < this->args.size()) {
            return Status{INSUFFICIENT_COUNT, i, this->args.size(), ""};
        }
        else {
            return Status{NO_ERROR, 0, 0, ""};
        }
    }

    void execute() {
        this->func(this->args);
    }
};

} // molecli::detail

#endif // MOLECLI_COMMAND_HPP