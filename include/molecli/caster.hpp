#ifndef MOLECLI_CASTER_HPP
#define MOLECLI_CASTER_HPP

#include <optional>
#include <string>
#include <algorithm>

namespace molecli::detail {

template <typename T>
bool cast(std::string &&token, void *output) { return false; }

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

    if (token.at(0) == '-') {
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

template <>
bool cast<float>(std::string &&token, void *output) {
    std::string copy = token;

    if (token.at(0) == '-') {
        copy.erase(0);
    }
    
    if (copy.find_first_not_of("0123456789.") != std::string::npos) {
        return false;
    }
    else if (std::count(token.begin(), token.end(), '.') > 1) {
        return false;
    }
    else {
        float *output_float = static_cast<float *>(output);
        *output_float = std::stof(token);
        return true;
    }
}

template <>
bool cast<double>(std::string &&token, void *output) {
    std::string copy = token;

    if (token.at(0) == '-') {
        copy.erase(0);
    }
    
    if (copy.find_first_not_of("0123456789.") != std::string::npos) {
        return false;
    }
    else if (std::count(token.begin(), token.end(), '.') > 1) {
        return false;
    }
    else {
        double *output_double = static_cast<double *>(output);
        *output_double = std::stof(token);
        return true;
    }
}

} // molecli::detail

#endif // MOLECLI_CASTER_HPP