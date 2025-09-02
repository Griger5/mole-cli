#ifndef MOLECLI_HELP_MESSAGE_HPP
#define MOLECLI_HELP_MESSAGE_HPP

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

std::ostream &operator<<(std::ostream &stream, const HelpMessage &msg) {
    stream << "\033[36m" << msg.name << "\033[39m" << "(";

    if (msg.type_names.size() > 0) {
        stream << msg.type_names[0];
    }

    for (std::size_t i = 1; i < msg.type_names.size(); i++) {
        stream << ", " << msg.type_names[i]; 
    }

    stream << ")\n";

    stream << "    " << msg.description << '\n';
    stream << "--------------------" << '\n';

    return stream;
}

} // molecli::detail

#endif // MOLECLI_HELP_MESSAGE_HPP