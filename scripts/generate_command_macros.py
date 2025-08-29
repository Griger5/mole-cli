from pathlib import Path
import sys

output_path = Path(__file__).parent / "../include/molecli/generated_command_macros.hpp"

max_commands = int(sys.argv[1]) + 1

with open(output_path, "w+") as f:
    f.write("#ifndef MOLECLI_GENERATED_COMMAND_MACROS_HPP\n#define MOLECLI_GENERATED_COMMAND_MACROS_HPP\n")

    f.write("#include \"cli.hpp\"\n#include \"command.hpp\"\n#include \"caster.hpp\"\n\n")

    for i in range(1, max_commands):
        f.write(f"#define ADD_COMMAND_{i}(cli_obj, command_name, desc, func")

        for j in range(i):
            f.write(f", type{j+1}")

        f.write(")\\\n")
        f.write("namespace molecli::detail::generated {\\\n")
        f.write("std::function<int(CLI &)> add_to_cli_##command_name = [](CLI &cli){cli.add_command(\\\n")
        f.write("#command_name,\\\ndesc,\\\n")
        f.write("Command{\\\n")
        
        f.write("[](const Args &args){func(")
        f.write(f"*static_cast<type1 *>(args[0])")
        for j in range(1, i):
            f.write(f", *static_cast<type{j+1} *>(args[{j}])")
        f.write(");},\\\n")

        f.write("Args{")
        f.write("new type1")
        for j in range(1, i):
            f.write(f", new type{j+1}")
        f.write("},\\\n")
        
        f.write("std::vector<Caster>{")
        f.write("cast<type1>")
        for j in range(1, i):
            f.write(f", cast<type{j+1}>")
        f.write("},\\\n")

        f.write("[](Args &args){")
        for j in range(i):
            f.write(f"delete static_cast<type{j+1} *>(args[{j}]);")
        f.write("}\\\n")

        f.write("});\\\n")
        f.write("return 1;};\\\n")
        
        f.write("std::vector<int> dummy_function_call{add_to_cli_##command_name(cli_obj)};}\n\n")

    f.write("#endif")