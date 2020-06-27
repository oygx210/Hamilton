cmake_minimum_required(VERSION 3.15.0)

# set the project name and version
project(Hamilton VERSION 0.0.1 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/build                    )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/bin  )

# Define some preprocessor directives
add_definitions(-DPROJECT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

# 
# Custom Options
#
option(BUILD_TESTS "Build the GNC Tests"
    ON
)

# Make sure we link against the correct visual studio runtime library
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Prepare includes
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extern/gcem/include")
list(APPEND APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extern/spice/include")
include_directories(${APP_INCLUDE_DIRS})

# add compile time math library
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/gcem")

# Add JPL Ephemeris library
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/spice")

# Add submodule libraries
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/ephemeris")

# Add the Gtest sources and the gnc tests
# TODO: Want to use gtest as an external library from within the test directory only
if (BUILD_TESTS)
    set(BUILD_GMOCK OFF)
    set(GTEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()
