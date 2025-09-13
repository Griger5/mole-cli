#ifndef MOLECLI_TOKENIZE_HPP
#define MOLECLI_TOKENIZE_HPP

#include <tuple>
#include <string>
#include <vector>

namespace molecli::detail {

std::tuple<std::string, std::vector<std::string>> tokenize(std::string &&line);

} // molecli::detail

#endif // MOLECLI_TOKENIZE_HPP