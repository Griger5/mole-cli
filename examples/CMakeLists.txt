cmake_minimum_required(VERSION 3.28)

project(molecli_examples)

find_package(molecli)

add_executable(01_basic 01_basic/main.cpp)
target_link_libraries(01_basic talpasoft::molecli)

add_executable(02_custom_class 02_custom_class/main.cpp)
target_link_libraries(02_custom_class talpasoft::molecli)