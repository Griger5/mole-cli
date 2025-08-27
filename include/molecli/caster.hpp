#ifndef MOLECLI_CASTER_HPP
#define MOLECLI_CASTER_HPP

#include <optional>
#include <string>

namespace molecli::detail {

template <typename T>
bool cast(std::string &&token, void *output) {return false;}

template <>
bool cast<bool>(std::string &&token, void *output) {
    if (!token.compare("0") || !token.compare("false") || !token.compare("FALSE")) {
        bool *output_bool = static_cast<bool *>(output);
        *output_bool = false;
        return true;
    }
    else if (!token.compare("1") || !token.compare("true") || !token.compare("TRUE")) {
        bool *output_bool = static_cast<bool *>(output);
        *output_bool = true;
        return true;
    }
    else {
        return false;
    }
}

template <>
bool cast<int>(std::string &&token, void *output) {
    std::string copy = token;

    if (copy.at(0) == '-') {
        copy.erase(0);
    }

    if (copy.find_first_not_of("0123456789") == std::string::npos) {
        int *output_int = static_cast<int *>(output);
        *output_int = std::stoi(token);
        return true;
    }
    else {
        return false;
    }
}

} // molecli::detail

#endif // MOLECLI_CASTER_HPP