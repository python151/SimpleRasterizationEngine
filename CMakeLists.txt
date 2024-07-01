# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(headers)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS})

# Add the executable
add_executable(MyProject source/main.c source/STL_Handler.c source/Scene.c)
target_link_libraries(MyProject ${SDL2_LIBRARIES})


