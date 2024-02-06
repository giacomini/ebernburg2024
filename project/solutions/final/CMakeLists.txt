cmake_minimum_required(VERSION 3.16)
project(daisyworld VERSION 0.1.0)

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

add_executable(daisyworld main.cpp patch.cpp world.cpp)

# To disable testing, pass the option -DBUILD_TESTING=OFF to the cmake configuration phase
# If testing is enables...
if (BUILD_TESTING)

  # add executable daisyworld.t
  add_executable(daisyworld.t test.cpp patch.cpp world.cpp)
  # add executable daisyworld.t to the tests' list
  add_test(NAME daisyworld.t COMMAND daisyworld.t)

endif()
