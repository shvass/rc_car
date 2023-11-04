# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(rc_car)



find_package(Doxygen
    COMPONENTS doxygen
    OPTIONAL_COMPONENTS mscgen dia)

# checks for documentation build
if(${Doxygen_FOUND})

    set(DOXYGEN_EXCLUDE_PATTERNS
    */build/*
    */out/*
    )

    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)

    doxygen_add_docs(rc_car_docs  ${CMAKE_SOURCE_DIR} ALL )

endif()