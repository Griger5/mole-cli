#ifndef CLI_BRARY_TYPING_UTIL_HPP
#define CLI_BRARY_TYPING_UTIL_HPP

#include <functional>
#include <memory>
#include <vector>

namespace talpasoft::detail {

enum class arg_type {
    Bool = 0,
    Int = 1,
    Float = 2,
    Double = 3,
    Char = 4,
    String = 5,
    Custom = 6
};

template <typename T>
constexpr arg_type get_type() {
    if constexpr (std::is_same_v<T, bool>) {
        return arg_type::Bool;
    }
    else if constexpr (std::is_same_v<T, int>) {
        return arg_type::Int;
    }
    else if constexpr (std::is_same_v<T, float>) {
        return arg_type::Float;
    }
    else if constexpr (std::is_same_v<T, double>) {
        return arg_type::Double;
    }
    else if constexpr (std::is_same_v<T, char>) {
        return arg_type::Char;
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        return arg_type::String;
    }
    else {
        return arg_type::Custom;
    }
}

} // talpasoft::detail

#endif // CLI_BRARY_TYPING_UTIL_HPP