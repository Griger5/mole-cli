#ifndef MOLECLI_COMMAND_HPP
#define MOLECLI_COMMAND_HPP

#include "typing_utils.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <tuple>

namespace molecli {

namespace detail {

struct BaseArgs {virtual ~BaseArgs() = default;};

}

class Command final {
private:
    std::function<void(detail::BaseArgs *)> func;
    std::unique_ptr<detail::BaseArgs> args;
    size_t arg_count;
    std::vector<detail::arg_type> arg_types;

public:
    Command() = default;

    Command(std::function<void(detail::BaseArgs *)> f, std::vector<detail::arg_type> v) {
        this->func = f;
        this->arg_types = v;
        this->arg_count = v.size();
    }
};

} // molecli

#endif // MOLECLI_COMMAND_HPP