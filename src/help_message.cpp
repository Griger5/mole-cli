#include "molecli/help_message.hpp"

namespace molecli::detail {

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