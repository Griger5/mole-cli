#ifndef MOLECLI_CASTER_HPP
#define MOLECLI_CASTER_HPP

#include <optional>
#include <string>

namespace molecli::detail {

struct AbstractCaster {
    virtual std::optional<int> cast(std::string &&token) = 0;
};

template <typename T>
struct Caster final : public AbstractCaster {};

template <>
struct Caster<int> final : public AbstractCaster {
    std::optional<int> cast(std::string &&token) override {
        std::string copy = token;
        
        if (copy.at(0) == '-') {
            copy.erase(0);
        }

        if (copy.find_first_not_of("0123456789") == std::string::npos)
            return std::make_optional<int>(std::stoi(token));
        else
            return std::nullopt;
    }
};

} // molecli::detail

#endif // MOLECLI_CASTER_HPP