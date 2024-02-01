cmake_minimum_required(VERSION 3.16)
project(mandelbrot_sfml VERSION 0.1.0)

# enable support for testing, including the definition of the BUILD_TESTING
# option, used below
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# require the use of C++17, without compiler-specific non-standard extension
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

# enable the address sanitizer and the undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG
       " -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG
       " -fsanitize=address,undefined -fno-omit-frame-pointer")

# request the use of the SFML library, version 2.5, limited to its graphics component (and direct and indirect
# dependencies)
find_package(
  SFML 2.5
  COMPONENTS graphics
  REQUIRED)

add_executable(mandelbrot main.cpp)
target_link_libraries(mandelbrot PRIVATE sfml-graphics)

if(BUILD_TESTING)
  add_executable(all.t all.t.cpp complex.t.cpp)
  add_test(NAME all.t COMMAND all.t)
endif()
