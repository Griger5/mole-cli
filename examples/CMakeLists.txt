cmake_minimum_required(VERSION 3.28)

project(molecli_examples)

find_package(molecli)

add_executable(01_basic 01_basic/main.cpp)
target_link_libraries(01_basic talpasoft::molecli)

add_executable(02_custom_class 02_custom_class/main.cpp)
target_link_libraries(02_custom_class talpasoft::molecli)

add_executable(03_static_vars 03_static_vars/main.cpp)
target_link_libraries(03_static_vars talpasoft::molecli)

add_executable(04_multi_cli 04_multi_cli/main.cpp)
target_link_libraries(04_multi_cli talpasoft::molecli)

add_executable(05_script 05_script/main.cpp)
target_link_libraries(05_script talpasoft::molecli)