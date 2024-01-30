# Project session 1 (Monday)

- [Project session 1 (Monday)](#project-session-1-monday)
  - [Warm-up](#warm-up)
    - [Working area setup](#working-area-setup)
    - [Code formatting with _clang-format_](#code-formatting-with-clang-format)
    - [Getting familiar with the GCC C++ compiler](#getting-familiar-with-the-gcc-c-compiler)
    - [Solutions](#solutions)

## Warm-up

### Working area setup

As a first step, we warmly suggest you to **create a dedicated folder** hosting
the project that you are going to develop, and move into that folder:

```bash
$ pwd
/home/battilan
$ mkdir project
$ cd project
$ pwd
/home/battilan/project
$ ls -la
total 0
drwxr-xr-x  2 battilan  staff  64 27 Sep 10:04 .
drwxr-xr-x  3 battilan  staff  96 27 Sep 10:04 ..
```

From now onwards we will refer to the `project/` folder as **working area**.

### Code formatting with _clang-format_

Keeping code formatted, in a consistent and understandable way, is **is of prime
importance** in virtually any modern code development workflow.
Besides allowing to identify on the fly a few types of errors (e.g.: an
inconsistent or incorrect use of parentheses), _code formatters_ help **multiple
developers collaborating to a single project** keeping a **consistent coding
style**.

The code formatting tool that we will use is
[_clang-format_](https://clang.llvm.org/docs/ClangFormat.html).

To allow for code formatting policies to vary between different projects,
clang-format looks for the presence of a dedicated configuration file
(`.clang-format`) within the working area defining the project's scope.

Generate a `.clang-format` configuration file within your working area by
issuing:

```bash
$ clang-format --dump-config -style=google > .clang-format
$ head .clang-format 
---
Language:        Cpp
# BasedOnStyle:  Google
AccessModifierOffset: -1
AlignAfterOpenBracket: Align
AlignArrayOfStructures: None
AlignConsecutiveAssignments:
  Enabled:         false
  AcrossEmptyLines: false
  AcrossComments:  false
```

> :exclamation: In the above example, we decided to generate a configuration
> file using the google _style preset_.
>
> :exclamation: You can look at the
> [documentation](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
> to learn more about the different clang-format formatting styles and options.

At this point open your favourite editor, (e.g. issue `code .` to open VSCode
from your working area), create a new file named `hello.cpp` and fill it with
the following _c++ snippet_:

```c++
int main ( ) {



    }
```

Then save it and, from the terminal, run:

```bash
$ clang-format -i hello.cpp
```

The content of `hello.cpp` should now look like this:

```c++
int main() {}
```

> :exclamation: The `-i` option tells `clang-format` to perform in-place editing
> of the file(s) it processes.

Clang-format integrations are available for a wide list of editors/IDEs
[are available](https://clang.llvm.org/docs/ClangFormat.html#vim-integration).
They allow to perform code formatting using clang-format directly during the
editing process.

> :exclamation: We warmly suggest that, in your free time, you spend a few
> minutes learning how to run code formatting from your favourite editor/IDE.
>
> For example, once properly configured, VSCode clang-format key-bindings are:
>
> - Linux: <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>I</kbd>;
> - Windows: <kbd>Alt</kbd> + <kbd>Shift</kbd> + <kbd>F</kbd>;
> - Mac OS: <kbd>Option</kbd> + <kbd>Shift</kbd> + <kbd>F</kbd>.

### Getting familiar with the GCC C++ compiler

During these classes we will use the `g++` (C++) compiler from
[GCC](https://gcc.gnu.org/) (the GNU Complier Collection).

Please test your setup by compiling the `hello.cpp` program using the following
command:

```bash
$ g++ -Wall -Wextra hello.cpp -o hello
```

> :exclamation: The `-Wall -Wextra` options instruct the compiler to report
> about potential problems (_warnings_) which do not strictly preventing a
> correct compilation, but may indicate potential conceptual issues.
>
> :exclamation: Mac OS users should use `g++-13` instead of `g++` to compile
> their programs (options and arguments remain unchanged).
> :exclamation: Mac OS users, please check what C++ compiler the `g++` command
> invokes in your case.

If everything went as expected, you should see new executable file, named
`hello`, in your working area:

```bash
$ file hello
hello: Mach-O 64-bit executable arm64
```

> :exclamation: The output of the `file` command is expected to vary, depending
> on both the operating system you are using and the architecture of your
> laptop, but it should always refer to an executable file.

Understanding compiler errors is _a must_ for C++ programmers. If you are not
already familiar with `g++`, we suggest you replace the content of `hello.cpp`
with the following snippet:

```c++
#include <iosteram>

int main()
{
  std::cout << "Insert your name:\n";
  std::string name
  cin >> name;
  std::cout << "Hello " << name << '\n':
}
```

compile and **fix one by one the bugs** that were **introduced deliberately**.
While doing that pay attention to what is reported by the compiler's error
messages.

### Solutions

The day following each project session, we will release proposals of solutions
for the exercises and activities that tok place in class [here](solutions/).
