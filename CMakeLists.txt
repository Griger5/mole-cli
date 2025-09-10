cmake_minimum_required(VERSION 3.28)

project(molecli VERSION 0.5.0)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if(NOT MAX_COMMAND_ARGS)
    set(MAX_COMMAND_ARGS 10)
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter)

execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import shutil; shutil.copytree('external/isocline/src/', 'include/molecli/external/isocline/src/', dirs_exist_ok=True)")
execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import shutil; shutil.copytree('external/isocline/include/', 'include/molecli/external/isocline/include/', dirs_exist_ok=True)")
execute_process(
    COMMAND ${Python3_EXECUTABLE} "-c" "import sys; print(''.join([line if not 'setlocale' in line else '  const char* loc = setlocale(LC_ALL,\"C\");' for line in sys.stdin]))"
    INPUT_FILE ${CMAKE_SOURCE_DIR}/include/molecli/external/isocline/src/tty.c
    OUTPUT_FILE ${CMAKE_SOURCE_DIR}/include/molecli/external/isocline/src/tty2.c
)
execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import os; os.remove('include/molecli/external/isocline/src/tty.c'); os.rename('include/molecli/external/isocline/src/tty2.c', 'include/molecli/external/isocline/src/tty.c')")

include(GNUInstallDirs)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE talpasoft::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(hello_test
    "tests/hello_test.cpp"
)

target_link_libraries(hello_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)