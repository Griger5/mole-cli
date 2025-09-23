cmake_minimum_required(VERSION 3.28)

project(molecli VERSION 0.7.0)

add_library(${PROJECT_NAME}
    src/cli.cpp
    src/command.cpp
    src/help_message.cpp
    src/tokenize.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE include)

if(NOT NO_DEPS)
    target_include_directories(${PROJECT_NAME} PRIVATE external/isocline/include)

    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    execute_process(
        COMMAND ${Python3_EXECUTABLE} "-c" "import sys; print(''.join([line if not 'setlocale' in line else '''  const char* loc = setlocale(LC_ALL,\"C\");\n''' for line in sys.stdin]))"
        INPUT_FILE ${CMAKE_SOURCE_DIR}/external/isocline/src/tty.c
        OUTPUT_FILE ${CMAKE_SOURCE_DIR}/external/isocline/src/tty2.c
    )
    execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import os; os.remove('external/isocline/src/tty.c'); os.rename('external/isocline/src/tty2.c', 'external/isocline/src/tty.c')")

    target_sources(${PROJECT_NAME} PRIVATE external/isocline/src/isocline.c)
else()
    add_compile_definitions(NO_DEPS)
endif()

include(GNUInstallDirs)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
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

if (BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        EXCLUDE_FROM_ALL
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(molecli_tests
        tests/test_caster.cpp
        tests/test_cli.cpp
        tests/test_command.cpp
        tests/test_command_s.cpp
        tests/test_help_message.cpp
        tests/test_tokenize.cpp
    )

    target_compile_features(molecli_tests PUBLIC cxx_std_17)

    target_link_libraries(molecli_tests ${PROJECT_NAME} GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(molecli_tests)

    add_executable(molecli_check_leaks tests/check_leaks.cpp)
    target_link_libraries(molecli_check_leaks ${PROJECT_NAME})
endif()