cmake_minimum_required(VERSION 3.28)

project(molecli_examples)

find_package(molecli)

add_executable(01_basic 01_basic/main.cpp)
target_link_libraries(01_basic talpasoft::molecli)