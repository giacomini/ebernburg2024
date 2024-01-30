# Project session 2 (Tuesday)

- [Project session 2 (Tuesday)](#project-session-2-tuesday)
  - [Getting started with doctest](#getting-started-with-doctest)
    - [Exercise 1: Implementation and test of the `sum_n` function](#exercise-1-implementation-and-test-of-the-sum_n-function)
    - [Exercise 2: Least Common Multiple](#exercise-2-least-common-multiple)
  - [First steps on Daisyworld](#first-steps-on-daisyworld)
    - [Compute the local heating](#compute-the-local-heating)
    - [Diffusion process](#diffusion-process)

As a first step we will try to get comfortable with the
[_doctest_](https://github.com/doctest/doctest) header only library, using it to
verify the correct behavior of a function.

Later on we will start working on some functions which dictate the basic
behavior of our Daisyworld toy model.

## Getting started with doctest

First download [doctest.h](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h)
 in your working area with the following command:

```bash
$ pwd
/home/srossi/project
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Then, create a `sum_n.cpp` file, containing the following code snippet:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the sum_n function") { }
```

Format the code with _clang-format_, save, compile and run it:

```bash
$ g++ -Wall -Wextra sum_n.cpp -o sum_n 
$ ./sum_n 
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 0 | 0 passed | 0 failed |
[doctest] Status: SUCCESS!
```

Even with no main function the program works, thanks to the pre-processor
directive:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
```

which defines an identifier (`DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`) that signals
_doctest_ to prepare a `main` function that runs the _test case_ implemented:

```c++
TEST_CASE("Testing the sum_n function") { }
```

Since in the example the test case is empty, the test runs successfully.

### Exercise 1: Implementation and test of the `sum_n` function

At this point, before implementing the `sum_n` function in any way, let's think
about how it should be **syntactically structured** and how it **should behave**.

> :exclamation: In addition to ensuring that the code doesn't "break" during
> different development phases, tests are essential to ensure that a function
> behaves as we want it to.
>
> :warning: Therefore, once the expected behavior is defined, it is good practice
to **implement the tests before implementing the code** they are meant to verify.

The `sum_n` function should:

- accept a **non-negative integer** as an **argument**
- **return an integer** with the **result of the summation**

In C++, this translates to `int sum_n(int n) { ... }`.

However, **in this implementation**, it is formally allowed to pass a **negative
integer `n`** as argument to the `sum_n` function.
Let's decide that, if a **negative argument is provided**, `sum_n` should
**always return the integer literal `-1`**.

At this point, we have enough information to define the expected behavior of
`sum_n`.

Before implementing it, let's proceed to **implement tests**:

- returns the expected values in a couple of cases that we will calculate
  "by hand" in advance (e.g., `n=9` and `n=10`)
- returns `0` for the case `n=0`
- returns `-1` when we input a couple of negative numbers (e.g., `n=-4` and
  `n=-5`)

In doctest, such checks are expressed using the following syntax:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the sum_n function") {
  CHECK(sum_n(10) == 55);
  ...
}
```

> :warning: Complete yourself the implementation of the tests.

Now, if you format, save, and try to compile, you should get:

```bash
$ g++ -Wall -Wextra sum_n.cpp -o sum_n 
In file included from sum_n.cpp:3:
sum_n.cpp: In function 'void DOCTEST_ANON_FUNC_8()':
sum_n.cpp:6:9: error: 'sum_n' was not declared in this scope
    6 |   CHECK(sum_n(10) == 55);
      |         ^~~~~
...
```

> :warning: Implement the missing function and repeat the compilation until the
> program works as expected.

### Exercise 2: Least Common Multiple

In this exercise, we want to write a function that calculates the least common
multiple between two integers.

> :exclamation: Let's recall the definition: the **least common multiple** of
> two integers _a_ and _b_, denoted as `lcm(a,b)`, is the smallest **positive**
> integer that is a multiple of both _a_ and _b_.
> In the particular case where either _a_ or _b_ is equal to zero, then we
> define `lcm(a,b)` as zero.

Let's start by creating a file called `lcm.cpp`, where we will write the code.

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int lcm(int a, int b) {
  ...
}

TEST_CASE("Testing the lcm function") {
  ...
}

```

As before, let's begin by thinking about **the behavior that the function should
have**, and then about **the tests to implement**:

- We can start by defining the behavior of the `lcm(a,b)` function for some
  positive integers (e.g., `a=12` and `b=16`)
- When `a=0` and/or `b=0`, we want `lcm(a,b)` to return `0`
- Finally, we want to verify that the function behaves correctly when one
  of _a_ or _b_ is less than `0`, or both are.

For the purposes of this lab, we suggest implementing the calculation using
a "brute force" approach:

- Initialize the integer variable `max` to the maximum value between `a` and `b`
  (for example, you can initially do this using an `if` statement)
- Initialize the integer variable `result` to the value of `max`
- Check if `result % a` and `result % b` are both `0`; if not, increment `result`
  by `max` and repeat the check until satisfied (you can use a while loop)

Format, save, compile the code, and run it.

If you chose the suggested approach, the function probably satisfies the
first test case but may fail the checks where `a` and/or `b` are less than or
equal to `0`.

> :exclamation: Take care of modifying the `lcm(a,b)` function to handle
> these cases appropriately.
> :exclamation: To do this, it might be useful to use some functions defined
> in the `cmath` library, such as `std::abs`.

## First steps on Daisyworld

Let's take our first steps on Daisyworld!

### Compute the local heating

On this planet, the temperature of a given patch of land depends on the solar
luminosity ($L_S$) and the local albedo ($\alpha$) of the patch.

In particular, the temperature of a local patch is calculated in discrete steps
as an average of the current local temperature and the local heating due to the
absorbed luminosity ($L_A$).

The absorbed luminosity is computed as:

$$
L_A = (1-\alpha)L_S
$$

The local heating ($\Delta T$) is given by:

$$
\Delta T = 72\times ln(L_A) + 80
$$

The local heating is calculated as logarithmic function of the solar luminosity
where an absorbed luminosity of 1 yields a local heating of 80 ºC,  
and an absorbed luminosity of .5 yields a local heating of approximately 30 ºC,
and a absorbed luminosity of 0.01 yields a local heating of approximately -252 ºC.

> :warning: The formula presented right above is somehow arbitrary. We decided
> to stick to it as the purpose of this corse is learning basics of C++, hence
> we decided to somehow compromise accuracy to favor simplicity.

Implement and test the function `local_heating` that given the `solar_luminosity`
and `albedo` returns the _local heating_ given by the formulas above.

> :warning: **Implement the tests first**, and only afterward the function
> itself.
>
> :warning: The use of a **logarithm** imposes a **specific limitation** on the
> valid range of absorbed luminosity values, make sure to test for the special
> case.  
> Fallback to 80 ºC in that case.

### Diffusion process

In our Daisyworld, a patch of land influences the temperature of the neighboring
lands.

> :warning: A cell in a 2D grid has eight neighbors, in both cardinal and
> diagonal direction.
>
> For example in:
>
>```table
>1 2 3
>4 5 6
>7 8 9
>```
>
>- The cell 5 has eight neighbors: 1, 2, 3, 4, 6, 7, 8, 9;
>- The cell 2 has 5 neighbors: 1, 3, 4, 5, 6;
>- The cell 9 has 3 neighbors: 5, 6, 8;

The diffusion process goes like this:

- For every cell with temperature `t` in the grid, a cell "gives part of it's
  temperature" to all the neighboring cells.
- The portion of temperature diffused is give by a `diffusion_rate` parameter
  that goes from `0.` (no temperature is diffused to the neighbors)
  to `1.` (all temperature is diffused to the neighbors).
- Every neighbor receives one eight of the diffused temperature (no matter the
  number of neighbors).
- The cell retains all the leftover temperature.
- Regardless of topology, the sum of temperatures will be conserved across the
  grid.

Implement the `diffusion` function that takes as arguments a vector of
temperatures, the size (dimension of one side) of the grid and returns a new
vector with the new temperatures after diffusion.

> :warning: A grid of temperatures can be represented with a single vector of
> doubles, and its size (a single `int` for a square matrix)  
> Given the index `idx`, the row and column number are the _quotient_ and the
> _remainder_ of the integer division `idx/size`, respectively.
>
> :exclamation: Given the `row` and `column` you can obtain the index as
> `idx = row * size + col`
>
> :exclamation: A broader overview of the functionalities provided by
> `std::vector` will be presented in class on Thursday. For now you can
> assume that you can create a `std::vector` of size _N_, initialized by default
> to _0_,  by writing something like:
>
> ```c++
> int N{10};
> std::vector<double> my_vector{N};
> ```
>

The `diffusion` function should satisfy the following tests:

```c++
TEST_CASE("Testing diffusion") {
  SUBCASE("Uniform temperature") {
    SUBCASE("Temperature=0") {
      double diffusion_rate = 1.;
      double temperature = 0.;
      std::vector<double> temperatures(9, temperature);
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) ==
            doctest::Approx(sum_all(temperatures)));
      for(auto const& t:diffused_temperature){
        CHECK(t == doctest::Approx(0.));
      }
    }

    SUBCASE("Temperature=8") {
      double diffusion_rate = 1.;
      double temperature = 8.;
      std::vector<double> temperatures(9, temperature);
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) == doctest::Approx(sum_all(temperatures)));
      for(auto const& t:diffused_temperature){
        CHECK(t == doctest::Approx(8.));
      }
    }
  }
  SUBCASE("Central temperature diffusion") {
    SUBCASE("Diffusion rate 1") {
      double diffusion_rate = 1.;
      std::vector<double> temperatures{0., 0., 0., 0., 8., 0., 0., 0., 0.};
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) == doctest::Approx(8.));
      CHECK(diffused_temperature[0] == doctest::Approx(1.));
      CHECK(diffused_temperature[1] == doctest::Approx(1.));
      CHECK(diffused_temperature[2] == doctest::Approx(1.));
      CHECK(diffused_temperature[3] == doctest::Approx(1.));
      CHECK(diffused_temperature[4] == doctest::Approx(0.));
      CHECK(diffused_temperature[5] == doctest::Approx(1.));
      CHECK(diffused_temperature[6] == doctest::Approx(1.));
      CHECK(diffused_temperature[7] == doctest::Approx(1.));
      CHECK(diffused_temperature[8] == doctest::Approx(1.));
    }
    SUBCASE("Diffusion rate 0.5") {
      double diffusion_rate = 0.5;
      std::vector<double> temperatures{0., 0., 0., 0., 8., 0., 0., 0., 0.};
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) == doctest::Approx(8.));
      CHECK(diffused_temperature[0] == doctest::Approx(0.5));
      CHECK(diffused_temperature[1] == doctest::Approx(0.5));
      CHECK(diffused_temperature[2] == doctest::Approx(0.5));
      CHECK(diffused_temperature[3] == doctest::Approx(0.5));
      CHECK(diffused_temperature[4] == doctest::Approx(4));
      CHECK(diffused_temperature[5] == doctest::Approx(0.5));
      CHECK(diffused_temperature[6] == doctest::Approx(0.5));
      CHECK(diffused_temperature[7] == doctest::Approx(0.5));
      CHECK(diffused_temperature[8] == doctest::Approx(0.5));
    }
    SUBCASE("Diffusion rate 0.") {
      double diffusion_rate = 0.;
      std::vector<double> temperatures{0., 0., 0., 0., 8., 0., 0., 0., 0.};
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) == doctest::Approx(8.));
      CHECK(diffused_temperature[0] == doctest::Approx(0.));
      CHECK(diffused_temperature[1] == doctest::Approx(0.));
      CHECK(diffused_temperature[2] == doctest::Approx(0.));
      CHECK(diffused_temperature[3] == doctest::Approx(0.));
      CHECK(diffused_temperature[4] == doctest::Approx(8));
      CHECK(diffused_temperature[5] == doctest::Approx(0.));
      CHECK(diffused_temperature[6] == doctest::Approx(0.));
      CHECK(diffused_temperature[7] == doctest::Approx(0.));
      CHECK(diffused_temperature[8] == doctest::Approx(0.));
    }
  }
  SUBCASE("Corner temperature diffusion") {
    SUBCASE("Diffusion rate 1") {
      double diffusion_rate = 1.;
      std::vector<double> temperatures{8., 0., 0., 0., 0., 0., 0., 0., 0.};
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) == doctest::Approx(8.));
      CHECK(diffused_temperature[0] == doctest::Approx(5.));
      CHECK(diffused_temperature[1] == doctest::Approx(1.));
      CHECK(diffused_temperature[2] == doctest::Approx(0.));
      CHECK(diffused_temperature[3] == doctest::Approx(1.));
      CHECK(diffused_temperature[4] == doctest::Approx(1.));
      CHECK(diffused_temperature[5] == doctest::Approx(0.));
      CHECK(diffused_temperature[6] == doctest::Approx(0.));
      CHECK(diffused_temperature[7] == doctest::Approx(0.));
      CHECK(diffused_temperature[8] == doctest::Approx(0.));
    }
  }
}
```

> :exclamation: Read carefully the tests and try to understand what is the
> expected interface and implement the function so that the tests all succeed.
>
> :exclamation: The `doctest::Approx` wrapper, very useful when dealing with
> rounding coming from floating point calculations, is documented
> [here](https://github.com/doctest/doctest/blob/master/doc/markdown/assertions.md#floating-point-comparisons)
>
> :question: What is the purpose of the `sum_all` function? How would you
> implement it?
>
> :warning: Before implementing it, feel free to add any additional test you
> think is necessary to verify that `diffuse` functions correctly.
