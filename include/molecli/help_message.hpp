#ifndef MOLECLI_HELP_MESSAGE_HPP
#define MOLECLI_HELP_MESSAGE_HPP

#include <ostream>
#include <vector>
#include <string>

namespace molecli::detail {

class HelpMessage {
private:
    std::string name;
    std::vector<std::string> type_names;
    std::string description;

public:
    HelpMessage() = default;

    HelpMessage(const std::string &cmd_name, const std::string &desc, const std::vector<std::string> &type_names_vec) : name{cmd_name}, 
        type_names{type_names_vec},  
        description{desc} {}

    friend std::ostream &operator<<(std::ostream &stream, const HelpMessage &msg);
};

} // molecli::detail

#endif // MOLECLI_HELP_MESSAGE_HPP