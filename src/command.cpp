#include "molecli/command.hpp"

namespace molecli::detail {

Command::Command(std::function<void(const Args &)> &&func, Args &&args, std::vector<Caster> &&casts, std::vector<DeallocFunc> &&dealloc, std::vector<std::string> &&names) {
    this->func = std::move(func);
    this->args = std::move(args);
    this->casters = std::move(casts);
    this->dealloc_args = std::move(dealloc);
    this->type_names = std::move(names);
}

void Command::dealloc() {
    for (std::size_t i = 0; i < this->args.size(); i++) {
        this->dealloc_args[i](this->args, i);
    }
}

Command::Status Command::load_arguments(std::vector<std::string> &&tokens) {
    std::size_t i = 0;
    bool cast_success;

    if (tokens.size() > this->args.size()) {
        return Status{TOO_MANY_ARGS, tokens.size(), this->args.size(), ""};
    }

    for (auto &&token : tokens) {
        if (!this->casters[i](std::move(token), this->args[i])) {
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

void Command::execute() {
    this->func(this->args);
}

} // molecli::detail