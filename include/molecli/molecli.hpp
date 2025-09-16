#ifndef MOLECLI_MOLECLI_HPP
#define MOLECLI_MOLECLI_HPP

#include "cli.hpp"
#include "cli_s.hpp"

#define MOLECLI_ADD_CASTER(Type, caster_func) \
template <> \
bool molecli::detail::cast<Type>(std::string &&token, void *output) { \
    Type *output_type = static_cast<Type *>(output); \
    std::optional<Type> res = caster_func(std::move(token)); \
    if (res.has_value()) { \
        *output_type = res.value(); \
        return true; \
    } \
    else { \
        return false; \
    } \
}

#endif // MOLECLI_MOLECLI_HPP