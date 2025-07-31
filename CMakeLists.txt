cmake_minimum_required(VERSION 3.22)
project(sourcevault LANGUAGES CXX)

# C++20 standard enforcement
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Debug
set(COMMON_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -Wshadow)

# Enable Qt's meta-object compiler, resource compiler, and user interface compiler
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Core and Gui
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        Test)
add_subdirectory(src)

# Testing
option(BUILD_TESTING "Build the tests" ON)
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)

if (CMAKE_CONFIGURATION_TYPES OR (NOT CMAKE_BUILD_TYPE STREQUAL "Release"))
    add_subdirectory(tests)
else ()
    message(STATUS "Skipping tests in Release build.")
endif ()
