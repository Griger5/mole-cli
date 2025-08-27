#ifndef MOLECLI_COMMAND_HPP
#define MOLECLI_COMMAND_HPP

#include "typing_utils.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <tuple>

namespace molecli::detail {

struct BaseArgs {virtual ~BaseArgs() = default;};

class Command final {
private:
    std::function<void(BaseArgs *)> func;
    std::unique_ptr<BaseArgs> args;
    size_t arg_count;
    std::vector<arg_type> arg_types;

public:
    Command() = default;

    Command(std::function<void(BaseArgs *)> f, std::vector<arg_type> v) {
        this->func = f;
        this->arg_types = v;
        this->arg_count = v.size();
    }
};

} // molecli::detail

#endif // MOLECLI_COMMAND_HPP