# Project session 5 (Friday)

- [Project session 5 (Friday)](#project-session-5-friday)
  - [Final steps](#final-steps)
    - [Separating Daisyworld's code into multiple files](#separating-daisyworlds-code-into-multiple-files)
      - [Step one: moving `Patch` functionalities outside `daisyworld.cpp`](#step-one-moving-patch-functionalities-outside-daisyworldcpp)
      - [Step two: moving `Patch` definitions into `patch.cpp`](#step-two-moving-patch-definitions-into-patchcpp)
      - [Repeat the exercise for `World`](#repeat-the-exercise-for-world)
  - [Final refinements](#final-refinements)
    - [Printing daisies](#printing-daisies)
    - [Adding a `main` function](#adding-a-main-function)
  - [A brief introduction to _CMake_](#a-brief-introduction-to-cmake)

## Final steps

In this project session, you will put in practice the freshly acquired
knowledge on the _C++ compilation model_ by **separating the project components
into multiple files**.

Additionally, we will provide you instruction about how to complete some tiny
missing parts. They will allow you to properly implement a `main` that prints
an evolving _Daisyworld_ into the _Terminal_.

Finally we will briefly introduce basic concepts about the use of
[_CMake_](https://cmake.org/).

> :exclamation: Please note that, **today**, **you can follow the session even
> if you are far from completing your own project**.  
> In fact, you _could download our latest proposed solution_ and start from
> that.

### Separating Daisyworld's code into multiple files

#### Step one: moving `Patch` functionalities outside `daisyworld.cpp`

We suggest sto start by creating a new file, called `patch.hpp`, and move
all the project functionalities related to `Patch` into this new file, while
removing them from `daisyworld.cpp`.

> :warning: This also imply that you should move there the `Daisy` _enum class_
> as well as the `local_heating` _free function_.

When doing that, please pay attention to add to `patch.hpp` only the `#include`
that are needed by `Patch` and `local_heating`.

At this point add the following line into `daisyworld.cpp`:

```c++
#include "patch.hpp"
```

and compile:

```bash
$ g++ -Wall -Wextra -fsanitize=address,undefined daisyworld.cpp -o daisyworld
```

> :exclamation: Note that the compilation command remains unchanged: `patch.hpp`
> is now included by `daisyworld.cpp`, and **we still have one single
> _Translation Unit_ (TU) to be compiled**.

Try now to include a second time `patch.hpp`

```c++
#include "patch.hpp"
#include "patch.hpp"
```

and compile again.

You may face an error that should remind the ones who didn't, to add **_include
guards_ to protect against multiple inclusions**:

```c++
#ifndef PATCH_H
#define PATCH_H

...

#endif
```

Fix the problem, then to compile again.  
If everything works, please remove the second, unnecessary, inclusion.

#### Step two: moving `Patch` definitions into `patch.cpp`

At this point let's start moving, **one by one**, all the different definitions
into a (newly created) `patch.cpp` file.

> :exclamation: You can leave the definitions of _oneliners_, as `daisy()`,
> `temperature()` and `temperature(double)` into the `.hpp`

We suggest to **start the migration** with the _constructor_.

> :exclamation: When moved to the `cpp`, `Patch(Daisy daisy, double
> temperature, int age)` goes out of the class' scope.  
> You must prepend `Path::` to the _constructor_'s name.

When done, compile by running:

```bash
$ g++ -Wall -Wextra -fsanitize=address,undefined daisyworld.cpp patch.cpp -o daisyworld
```

> :exclamation: Note that this time **we have two TUs**, hence both need to be
> compiled

At this point you should get an error like (_the one we show is from Mac OS_):

```bash
duplicate symbol '__Z13local_heatingdd' in:
    /private/var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T/patch-a2c648.o
    /private/var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T/daisyworld-74e7f2.o
ld: 1 duplicate symbols
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

coming from the linker (`ld`), that complains about the fact that two
definitions of the `local_heating` function.
This **violates the _One Definition Rule_**, hence `ld` does not know which one
to use.

You can **solve the problem** by _inlining_ `local_heating`.

After this change, **the program should compile**.

> :question: Why this is not needed in the case of the `Patch` class member
> functions?

Please now proceed, by **moving the other functions one by one**, compiling at
the end of each step.

While doing that, please remember to move to the `#include` that are just needed
for a function's definition, from the `.hpp` to the `.cpp`.

> :question: Why are we suggesting to do that?
>
> :exclamation: This also applies to `#include <stdexcept>`, only needed by the
> _constructor_'s definition, please move it if you didn't already.

Note that, once migrated to the `.cpp`, a single `local_heating` definition
exists, hence you should remove the `inline` keyword.

> :question: Or may be you could leave `local_heating` in the `.hpp`, inlined,
> so that you have an example of the use of `inline`.

#### Repeat the exercise for `World`

Instructions for this part of the assignment are very simple: **proceed in the
same way for `Word`**!

> :exclamation: The `sum_all` free function is a helper actually only used
> for testing, we suggest to leave it together with the tests.

## Final refinements

### Printing daisies

To complete the simulation we need a way to **display the daisies on screen**.

You can do that by creating a `print` method in `World` that loops over all the
daisies and writes to `std::cout` a symbol for black daisies, white daisies and
barren lands.

> :exclamation: For an output similar to ours, use `"🌸"`, `"💮"`, and `"  "` for
> black, white and barren respectively.

However, you will quickly notice that we don't have a way to print those
directly, and we will need to add a check for the daisy type in the function.

To make the function more straightforward we can instead **overload** the
`operator<<` for `Patch`.

> :exclamation: try to implement the _ostream operator `<<`_ for `Patch`, and
> use it in the `print` method of `World`.

<details><summary>Possible interface</summary>

```c++
friend std::ostream& operator<<(std::ostream& os, const Patch& patch);
```

</details>

### Adding a `main` function

We are finally ready to _see_ the **results of our work**.

We are only missing a `main` file that deals with the execution.
Since we separated the tests from the rest of the code, write a `main.cpp` file
that includes our `.hpp` files and runs the simulation.

Our implementation, for example, is the following:

```c++
#include "patch.hpp"
#include "world.hpp"

int main()
{
  int size{20};
  double start_black_percentage{0.2};
  double start_white_percentage{0.2};
  int max_age{25};
  World world(size, start_black_percentage, start_white_percentage, max_age);
  int iterations{500};
  bool print_to_screen = true;
  if (size > 30) {
    print_to_screen = false;
  }
  simulate(world, iterations, print_to_screen);
}
```

As you can see, we made some modification to the interface of `simulate`.
It's no longer a method of `World`, and has a new boolean argument that
tells whether to print the daisies on screen or not.

Adapt the main file to your `simulate` function. In addition consider moving
some _parameters to the setup phase_, maybe passing them directly from the `main`
function.

> :warning: Printing to screen is a lengthy process, and takes in our case) more
> time than the simulation itself. To allow it to keep up with the simulation we
> can include a _brief pause_ in the process by using functions from the
> `thread` and the `chrono` libraries of the standard:
>
> ```c++
> std::this_thread::sleep_for(std::chrono::milliseconds(100));
> ```
>
> :exclamation: When printing to screen it would be nice to clear the terminal
> at each new step to have a more fluid visualization of the output.  
> We can use the
> `"\033c"` [escape sequence](https://en.wikipedia.org/wiki/ANSI_escape_code#In_shell_scripting)
> to reset the screen:
>
> ```c++
> std::cout << "\033c";
> ```

## A brief introduction to _CMake_

Since compiling our code has become quite an hassle, we will use _CMake_ to make
the work easier on us.

The following `CMakeLists.txt` file, describes the configuration for the
compilation of our project:

```cmake
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

# enable warnings
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

# enable the address sanitizer and the undefined-behavior sanitizer in debug mode
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
```

To make use of it, we first need to run the configuration. We can do that with
the command:

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
```

This will run the configuration in debug mode. To run it in release mode:

```bash
$ cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
```

To build (i.e. compile) the code and produce all the executables:

```bash
cmake --build build/debug
```

To run the executable (in this case daisyworld), we can run it from:

```bash
build/debug/daisyworld
```

This command will run the `main` function. To run the tests we can use:

```bash
cmake --build build --target test
```

Or directly by launching:

```bash
build/debug/daisyworld.t
```

To see the complete output of `doctest`.
