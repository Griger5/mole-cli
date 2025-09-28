# mole-cli
**mole-cli** is a cross-platform C++17 library for building command line interfaces. This library aims for easy installation, easy use, and quick integration with already existing projects. It has been tested on Linux, Windows and macOS through the use of *Github Actions*.

CLIs created by this library will give ample warnings if there is a wrong number of arguments passed to a command, or if the argument types do not match. Basic types are supported out-of-the-box (bool, int, float, double, char, string), and support for other types (user-defined included) can be easily added.

CLIs can also read input from different streams, so it is possible to pass instructions to it from a text file. This not only enables automatic testing without user input, but it can also be used to create programs akin to script runners.

The project heavily relies on template metaprogramming and some macros for flexibility and streamlined user experience.

[GoogleTest](https://github.com/google/googletest) library was used to provide unit-testing for all of the classes.

![License](https://img.shields.io/badge/License-MIT-blue.svg) ![Github Actions](https://img.shields.io/github/actions/workflow/status/Griger5/mole-cli/testing.yml) 
![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B) ![](https://img.shields.io/badge/CMake-064F8C?style=flat&logo=cmake&logoColor=fff) [![Linux OK](https://img.shields.io/static/v1?label=Linux&logo=Linux&color=yellow&message=%E2%9C%93)](https://en.wikipedia.org/wiki/Linux) [![macOS OK](https://img.shields.io/static/v1?label=macOS&logo=Apple&color=silver&message=%E2%9C%93)](https://en.wikipedia.org/wiki/macOS) [![Windows OK](https://img.shields.io/static/v1?label=Windows&logo=Windows&color=blue&message=%E2%9C%93)](https://en.wikipedia.org/wiki/Windows)

## Contents
1. [Requirements](#requirements)
2. [Build and install](#build-and-install)
3. [Using the library within your project](#using-the-library-within-your-project)
4. [The Basics](#the-basics)
5. [Custom classes as arguments](#custom-classes-as-arguments)
6. [Static variables](#static-variables)
7. [Nested CLIs](#nested-clis)
8. [Scripting](#scripting)
9. [Examples](#examples)

## Requirements
- Compiler supporting C++17 ([GCC](https://gcc.gnu.org/) for Linux, [MSVC](https://visualstudio.microsoft.com/downloads/) for Windows, [AppleClang](https://clang.llvm.org/) for macOS) 
- [CMake](https://cmake.org/) (>=3.28)
- [Python3](https://www.python.org/) (required for complete setup in CMake)

## Build and install
1. Clone and enter the repository:
```bash
git clone --recursive https://github.com/Griger5/mole-cli.git
cd mole-cli
```
The `--recursive` flag is needed if you want to download the external dependency ([isocline](https://github.com/daanx/isocline)).

2. Setup chosen build with *CMake*: 

Base setup:
```bash
cmake -S . -B build/
```
If you wish to also build the tests:
```bash
cmake -S . -B build/ -DBUILD_TESTS=1
```
If you don't want to use the dependency:
```bash
cmake -S . -B build/ -DNO_DEPS=1
```
NOTE: *Without the* `isocline` *library* you will lose the "quality of life" features like history or shortcuts.

3. Build and install the library:
```bash
cmake --build build/
sudo cmake --install build/
```

4. *(Optional)* Run tests:
```bash
ctest --test-dir build/
```

## Using the library within your project
### With CMake
**mole-cli** can be easily incorporated into CMake projects. Use the `find_package` to find the library:
```cmake
find_package(molecli)
```
 And link the library with your program:
```cmake
target_link_libraries(your_project talpasoft::molecli)
```

### Manual linking
You can also manually link `libmolecli.a` with your project. Example with `GCC`:
```bash
gcc example.cpp -lmolecli
```

## The Basics
All necessary components of the library are available in a single header file:
```c++
#include <molecli/molecli.hpp>
```
The core of the library is the `molecli::CLI` class. You can choose the prompt message during initialization:
```c++
molecli::CLI cli{"prompt"};
```
There are two built-in commands: `help` and `exit`. Calling `help` lists all available commands, like so:
```
AVAILABLE COMMANDS:
print(int, float)
    Prints two numbers to standard output
--------------------
help()/HELP()
    Lists all available commands
--------------------
exit()/EXIT()
    Exists the current CLI
--------------------
```
And the `exit` command simply exits the current CLI.

To add commands, use the `add_command` method:
```c++
template <typename ReturnType, typename... ArgTypes>
void CLI::add_command(std::string &&command_name, std::string &&description, std::function<ReturnType(ArgTypes...)> func);
```
`command_name` parameter sets the name for invoking the function in the CLI, and the description is used in the `help` command output. The function can also be passed as a function pointer, lambda or a functor.
```c++
void example_f(...) {...}
struct ExampleFunctor {
	void operator()(...) {...}
};

int main() {
	molecli::CLI cli;
	void(*f_ptr)(...) = example_f;
	std::function<void(...)> func_object = example_f;
	
	cli.add_command("function", "", example_f);
	cli.add_command("func_ptr", "", f_ptr);
	cli.add_command("std::function", "", func_object);
	cli.add_command("functor", "", ExampleFunctor());
	cli.add_command("lambda", "", [](...){...});
}
```

The CLI can then be ran using the `run_loop` method:
```c++
void CLI::run_loop(std::istream &i_stream = std::cin, std::ostream &o_stream = std::cout);
```
After calling this function the program will enter a loop, awaiting and executing commands, up until receiving the `exit` command.

#### Working example
```c++
#include <molecli/molecli.hpp>

#include <iostream>

void print(int a, float b) {
	std::cout << a << " " << b << '\n';
}

int main() {
	molecli::CLI cli{"example"};
	
	cli.add_command("print", "Print two numbers to standard output", print);
	
	cli.run_loop();
}
```
```
example> print 1 2.5
1 2.5
example> print 1
Warning: Insufficient number of arguments. Expected: 2, received: 1
example> print 1.5 2.5
Warning: Wrong type of argument #1. Argument type should be: int
example> print 1 2 3
Warning: Too many arguments. Expected: 2, received: 3
example> help
AVAILABLE COMMANDS:
print(int, float)
    Print two numbers to standard output
--------------------
help()/HELP()
    Lists all available commands
--------------------
exit()/EXIT()
    Exits the current CLI
--------------------
example> exit
```

## Custom classes as arguments
To use user-defined classes (or other types available in C++) as command arguments you have to create a custom caster for that type. The easiest way to do that is to define a function that parses a string into the given type, returning an `std::optional` construct to signify whether or not the string was valid.
```c++
std::optional<Type> type_from_string(std::string &&token);
```
You can use the following macro to add the needed caster:
```c++
MOLECLI_ADD_CASTER(Type, type_from_string);
```
Where `Type` is your class, and `type_from_string` is your defined function. Returning `std::nullopt` signals that the received string cannot be converted to the required type, and a proper warning will be displayed to the user.

After a caster is added, you do not need to alter your functions. They can be safely used in commands:
```c++
class Wrapper {...};
std::optional<Wrapper> wrapper_from_string(std::string &&token) {...}
MOLECLI_ADD_CASTER(Wrapper, wrapper_from_string);

void use_wrapper(Wrapper wrapper, float a) {...}

int main() {
	molecli::CLI cli;
	cli.add_command("use_wrapper", "", use_wrapper);
	cli.run_loop();
}
```
NOTE: *Providing a safe caster is the user's responsibility. You have to take care of all errors or crashes that may arise - the library only handles receiving* `std::nullopt` *as the "error" value*

## Static variables
"Static variables" are simply variables that persist between different command calls and can be accessed from within them. To create and access them you have to use a different version of the `CLI` object:
```c++
molecli::CLI_s<StaticTypes...> cli{"prompt"};
```
You define the types of the static variables through the template. If you want to add a command that uses static variables, you should also use a different method:
```c++
template <typename ReturnType, typename... ArgTypes>
void CLI_s<...>::add_command_s(std::string &&command_name, std::string &&description, std::function<ReturnType(std::shared_ptr<StaticVarsT>, ArgTypes...)> func);
```
Note the different signature of the function - it has to be defined with an additional argument. For convenience, a macro for that was added:
```c++
void example_func(MOLECLI_STATIC_VARS(cli_name), ...) {}
```
Where `cli_name` is the name of a previously defined object of type `CLI_s`. Now the function is ready to be added as a command. `CLI_s` objects can use both regular and "static" commands.

To access static variables inside a function, you can use another macro:
```c++
GetStaticVar(idx);
```
Where `idx` is the index of the variable. Trying to access an index larger than the amount of static variables will result in a compile error. The returned object is a modifiable l-value, so you can use it like this:
```c++
type &var = GetStaticVar(idx); // assign to a reference
type var = GetStaticVar(idx); // assing as a copy, modifying this value won't change the static one
GetStaticVar(idx) = ...; // direct access
```
You can also use the tuple unpacking syntax with another macro, to get access to all of the variables:
```c++
auto [var1, var2, ...] = GetStaticVars(); // you can use var1, var2 and others as you want
auto &[var1, var2, ...] = GetStaticVars();
```

## Nested CLIs
It is possible to have CLIs accessible from other CLIs. The nested CLI can have an entirely different roster of commands. To add one CLI to another, you need to create the nested one as an `std::shared_ptr`, and then use the following method:
```c++
void CLI::add_sub_cli(std::string &&cli_name, std::shared_ptr<CLI> cli_ptr);
```
Where `cli_name` is the command name by which you will switch to this CLI.

Both static and regular CLIs can be nested, and both static and regular CLIs can have nested CLIs.
```c++
using namespace molecli;

std::shared_ptr<CLI> sub_cli = std::make_shared<CLI>("sub");
std::shared_ptr<CLI_s<int>> sub_cli_s = std::make_shared<CLI_s<int>>("sub_s");

CLI cli;
CLI_s<double, float> cli_s;

cli.add_sub_cli("sub_cli1", sub_cli);
cli.add_sub_cli("sub_cli2", sub_cli_s);

cli_s.add_sub_cli("sub_cli1", sub_cli);
cli_s.add_sub_cli("sub_cli2", sub_cli_s);
```
Using `exit` inside a nested CLI will take you back to the primary one.
#### Example
```
> echo 2
2
> nested_cli
nested_cli>
nested_cli> echo2 2
4
nestec_cli> exit
> echo 3
3
> exit
```

## Scripting
By changing the input and output streams of a CLI to file streams, you can essentially change it into a script processor:
```c++
#include <molecli/molecli.hpp>

#include <fstream>

int main() {
	molecli::CLI cli;
	std::ifstream input{"input.txt"};
	std::ofstream output{"output.txt"};
	
	cli.add_command("echo", "Echo a string", [&](std::string str) {
		output << str << '\n';
	});
	cli.add_command("add", "Add two integers", [&](int a, int b) {
		output << a + b << '\n';
	});

	cli.run_loop(input, output);
}
```

```
// input.txt
echo woof
add 1 2
add a 2
add 1
help
exit

// output.txt
woof
3
Warning: Wrong type of argument #1. Argument type should be: int
Warning: Insufficient number of arguments. Expected: 2, received: 1
AVAILABLE COMMANDS:
add(int, int)
    Add two integers
--------------------
echo(std::string)
    Echo a string
--------------------
help()/HELP()
    Lists all available commands
--------------------
exit()/EXIT()
    Exits the current CLI
--------------------
```

## Examples
After successfully installing the library, you can build the examples with the following commands:
```bash
cmake -S examples/ -B examples/build/
cmake --build examples/build/
```
And then you can run them as follows:
```bash
./examples/build/01_basic
./examples/build/02_custom_class
./examples/build/03_static_vars
./examples/build/04_multi_cli
./examples/build/05_script
```