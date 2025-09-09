#ifndef MOLECLI_STATIC_VARS_HPP
#define MOLECLI_STATIC_VARS_HPP

namespace molecli::detail {

template <typename... StaticTypes>
class StaticVars {
private:
    std::tuple<StaticTypes...> vars;
    constexpr static std::size_t static_vars_len = sizeof...(StaticTypes);

public:
    template <std::size_t Idx>
    constexpr std::tuple_element_t<Idx, std::tuple<StaticTypes...>> &get() {
        static_assert(static_vars_len > Idx);
        return std::get<Idx>(vars);
    }
};

} // molecli::detail

#endif // MOLECLI_STATIC_VARS_HPP