cmake_minimum_required(VERSION 3.28)

project(cli-brary VERSION 0.0.1)

add_library(${PROJECT_NAME}
    "src/test.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_include_directories(${PROJECT_NAME} PRIVATE "external/rttg/include")
target_include_directories(${PROJECT_NAME} PRIVATE "external/isocline/include")

target_sources(${PROJECT_NAME} PRIVATE "external/isocline/src/isocline.c")

include(GNUInstallDirs)

target_include_directories(
    ${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

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