cmake_minimum_required(VERSION 3.30)
project(sourcevault LANGUAGES CXX)

# C++20 standard enforcement
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMMON_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)

# Enable Qt's meta-object compiler, resource compiler, and user interface compiler
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        Test)

add_subdirectory(src)

enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)
add_subdirectory(tests)

