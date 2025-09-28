#ifndef MOLECLI_STATIC_VARS_HPP
#define MOLECLI_STATIC_VARS_HPP

namespace molecli::detail {

template <typename... StaticTypes>
class StaticVars {
private:
    std::tuple<StaticTypes...> vars;

public:
    template <std::size_t Idx>
    constexpr std::tuple_element_t<Idx, std::tuple<StaticTypes...>> &get() {
        return std::get<Idx>(vars);
    }

    std::tuple<StaticTypes...> &get_tuple() {
        return this->vars;
    }
};

} // molecli::detail

#endif // MOLECLI_STATIC_VARS_HPP