#ifndef CLI_BRARY_COMMAND_HPP
#define CLI_BRARY_COMMAND_HPP

#include "typing_util.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <tuple>

namespace talpasoft {

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

} // talpasoft

#endif // CLI_BRARY_COMMAND_HPP