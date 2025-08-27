#ifndef MOLECLI_CASTER_HPP
#define MOLECLI_CASTER_HPP

#include <optional>
#include <string>

namespace molecli::detail {

template <typename T>
void cast(std::string &&token, void *output, bool &is_err) {}

template <>
void cast<int>(std::string &&token, void *output, bool &is_err) {
    std::string copy = token;

    if (copy.at(0) == '-') {
        copy.erase(0);
    }

    if (copy.find_first_not_of("0123456789") == std::string::npos) {
        int *output_int = static_cast<int *>(output);
        *output_int = std::stoi(token);
        is_err = false;
    }
    else {
        is_err = true;
    }
}

} // molecli::detail

#endif // MOLECLI_CASTER_HPP