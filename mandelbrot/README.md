# Example of use of CMake

This example requires the use of the SFML library for visualization. On Ubuntu 22.04, install it with

```shell
sudo apt install libsfml-dev
```

To configure in debug mode

```shell
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
```

To configure in release mode

```shell
$ cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
```

To build (i.e. compile) the code and produce all the executables

```shell
cmake --build build/debug
```

To run the executable (in this case `mandelbrot`)

```shell
build/debug/mandelbrot
```

To run the tests

```shell
cmake --build build --target test
```

To directly run the test executable (in this case `all.t`), with more details

```shell
build/debug/all.t
```
