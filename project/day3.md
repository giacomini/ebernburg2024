# Project session 3 (Wednesday)

- [Project session 3 (Wednesday)](#project-session-3-wednesday)
  - [Building Daisyworld with user-defined types](#building-daisyworld-with-user-defined-types)
    - [Model a `Patch` of land](#model-a-patch-of-land)
      - [Unit-testing also helps defining interfaces](#unit-testing-also-helps-defining-interfaces)
      - [Implementing `Patch`](#implementing-patch)
    - [A `World` to rule them all](#a-world-to-rule-them-all)

## Building Daisyworld with user-defined types

The goal of the third session is to apply the concepts you have learnt today
to build the different components of our _toy model_.
Mostly we will focus on understating how write _classes_ that implement the
behavior of the two main actors in this game: **a patch of land** and **the
world** itself.

> We know, Daisyworld _should be about daisies_, but in our proposal daisies
> are rather simple to model.
>
> :exclamation: From now move to a compilation command like:
>
> ```bash
> $ g++ -Wall -Wextra -fsanitize=address,undefined daisyworld.cpp -o daisyworld
> ```
>
> **NOTE**: Mac OS users may try to use the _clang_ C++ compiler in this case (
> `g++ -std=c++17`), but still should not expect the address sanitizer to be
> perfect in your case.

### Model a `Patch` of land

Every cell of our Daisyworld grid represents a **patch of land** that:

- is **exclusively** populated by _white daisies_, _black daisies_, or is a
  _barren area_;
- if populated with **daisies**, their **age matter**, as at a certain point
  **daisies die**;
  - in this model we assume that all daisies from  a given _patch_ have the
    same age  
    (i.e.: we can consider _age_ as a _property of a patch_);
  - we are not interested in knowing the actual age of a daisy, we just need "an
  - instrument" to let them age, and check if they die  
    (transforming the patch into a _barren area_);
- may **sprout** with daisies, if seeds of daisies from nearby patches reach it;
- has a **uniform temperature** that **may change** because of:
  - _diffusion effects_ from nearby patches;
  - _absorption of solar radiation_.

Additionally, we assume that:

- albedos for daisies and barren lands are:
  - **white daisies**: 0.75
  - **black daisies**: 0.25
  - **barren land**: 0.40

As this may be the first _C++ class_ you write on your own, we propose a
possible _class interface_:

```c++
class Patch {
  Daisy daisy_{Daisy::Barren};
  double temperature_{0.};
  int age_{0};

 public:
  Patch() ...;
  Patch(Daisy daisy, double temperature, int age) {...}

  Daisy daisy() const {...}
  double temperature() const {...}

  void temperature(double temperature) {...}

  void absorb_light(double solar_luminosity) {...}
  bool check_survival(int max_age) {...}
  void sprout(Daisy seed) {...}

};
```

> :exclamation: The `...` are, of course, no proper C++, they show you where you
> should be writing code to complete the class.
>
> :question: When one should be using `class` and when `struct`? Is there any
> reasons to prefer `class` in this case?
>
> :exclamation: Spend a little time checking details, e.g.: where did we put the
> use the keyword `const` and so on.

At this point, you might be thinking: _sound reasonable but **what is the C++
type of a `Daisy`**_ :question:

Well, in our proposed design `Daisy`, also describes _barren lands_, and should
be a type that is either `White`, `Black` or `Barren`.

Can you guess the type yourself?

<details><summary>Discover our proposal</summary>

We have used an `enum class`:

```c++
enum class Daisy { Black, White, Barren };
```

</details>

#### Unit-testing also helps defining interfaces

When we introduced you to _doctest_ we asked you to:

1. identify a function's signature (e.g.: `int sum_n(int)`);
2. write all tests based on how you wish such function to behave;
3. _only at this point_, implement the function.

With classes the flow is similar, but with an additional twist: **writing tests
helps you defining a class' interface**.

For example, thinking about how to implement a test for `absorb_light`, one
possible approach could be:

```c++
TEST_CASE("Test absorb_light") {
    Patch p(Daisy::Black, 10., 0);
    double solar_intensity{1.}
    p.absorb_light(solar_intensity);
    CHECK(p.temperature() == doctest::Approx(39.6).epsilon(0.01))
}
```

Which implies that you get an idea about:

- how your constructor should look like;
- the `absorb_light` interface;
- the need of a method that returns the `Patch` temperature.

_You don't need to do anything with this code snippet_, **we wrote those few
lines so that you are aware of the approach**.

Guidelines about how to proceed with the implementation are provided in the next
section.

#### Implementing `Patch`

What we suggest you to do now, is to proceed by adding to `daisyworld.cpp` the
proposed interface for `Patch` "incrementally":

1. add private variables and make code compile;
2. add, one by one, the tests right below;
3. implement what is needed to make the newly added tests pass;
4. repeat, from two, with the next proposed test.

<details><summary>Testing the constructor</summary>

```c++
TEST_CASE("Test patches") {
  Patch p0(Daisy::Barren, 0., 0);
  Patch p1(Daisy::White, 0., 0);
  Patch p2(Daisy::Black, 20., 25);

  CHECK(p0.daisy() == Daisy::Barren);
  CHECK(p1.daisy() == Daisy::White);
  CHECK(p2.daisy() == Daisy::Black);
  CHECK_THROWS(Patch{Daisy::Barren, 0., -1});
}
```

> You can look at
> [this page](https://github.com/doctest/doctest/blob/master/doc/markdown/assertions.md#exceptions)
> to learn more about `CHECK_THROWS`.

</details>

<details><summary>Testing the temperature retrieval</summary>

> :exclamation: Add the `SUBCASE` within the main `TEST_CASE`.
> You can look at
> [this page](https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md#test-cases-and-subcases)
> to learn more about `SUBCASE`.

```c++
  SUBCASE("Testing temperature retrieval") {
    CHECK(p0.temperature() == doctest::Approx(0.));
    CHECK(p1.temperature() == doctest::Approx(0.));
    CHECK(p2.temperature() == doctest::Approx(20.));
  }
```

</details>

<details><summary>Testing temperature setting</summary>

Writing this test is **left to you**!

</details>

<details><summary>Testing light absorption</summary>

> **NOTE**: let's agree that, in our simple model, the temperature after
> absorption is $T_{after} = (T_{before} + \Delta T_{from-heating}) / 2$;

```c++
  SUBCASE("Testing temperature") {
    p1.absorb_light(1.);
    CHECK(p1.temperature() == doctest::Approx(-9.9).epsilon(0.01));
    p2.absorb_light(1.);
    CHECK(p2.temperature() == doctest::Approx(39.6).epsilon(0.01));
  }
```

</details>

<details><summary>Testing daisies' ageing logic</summary>

> **NOTE**: daises die when their age is higher than **max_age**;
>
> **NOTE**: despite the name, let's agree that `check_survival` also takes care
> of ageing daisies by increasing `age_` by `1` **before** checking for
> survival.
>
> **NOTE**: let's agree that for _barren lands_ check_survival should return
> `false`;

```c++
  SUBCASE("Testing age") {
    int max_age = 25;
    CHECK(p0.check_survival(max_age) == false);
    CHECK(p1.check_survival(max_age) == true);
    CHECK(p2.check_survival(max_age) == false);
    CHECK(p2.daisy() == Daisy::Barren);
  }
```

</details>

<details><summary>Testing daisies' sprouting</summary>

This is also **left to you**.

> **NOTE**: remember to reset `age_` within this method.

</details>

### A `World` to rule them all

The `World` class will be used to **represent the entire grid** of our
_toy model_. The grid will contain all the _patches of land_ that fill
our Daisyworld.

This can be done exactly as when dealing with temperatures in the `diffuse`
function:

- a `std::vector` containing the _patches_;
- an `int` storing _number of matrix rows and columns_.

These (for now), will be the only private members of our World class.

When you first populate `World`, you must provide it the starting percentage
of `Black` and `White` daisies (the number of _barren land_ patches comes as a
consequence).

Start by implementing tests to verify that the population of the different
daisies and the amount of barren land corresponds to your expectation according
to the inputs you gave to the constructor.

> :warning: Don't worry if `Daisies`, aren't, at present, distributed randomly
> over the grid. We will cover this tomorrow.
>
> :warning: You will probably need to implement a method to expose the vector,
> to be able to validate it's content.

<details><summary>Discover some of our tests</summary>

```c++
TEST_CASE("Test World") {
  World world1(4, 0.5, 0.25);
  auto patches1{world1.patches()};
  SUBCASE("Population count") {
    int count_black{0};
    int count_white{0};
    int count_barren{0};
    for (auto const& p : patches1) {
      if (p.daisy() == Daisy::Black) ++count_black;
      if (p.daisy() == Daisy::White) ++count_white;
      if (p.daisy() == Daisy::Barren) ++count_barren;
    }
    CHECK(count_black == 8);
    CHECK(count_white == 4);
    CHECK(count_barren == 4);
}
```

</details>

Based on the description and the tests you just implemented, try to write the
interface of `World`, as we did for you in the case of `Patch`.

<details><summary>Discover our proposal</summary>

```c++
 private:
  int size_;
  std::vector<Patch> patches_;

 public:
  World(int size, double start_black_perc, double start_white_perc) {...}
  std::vector<Patch> patches() const {...}
```

</details>

With the interface defined, **try now to implement the constructor** and the
`patches` method.

Proceed iteratively until all your tests pass.

We then ask you to **add the some of the methods** needed to run the simulation.

We already, implemented and tested the `diffuse` function, but now we need to
adapt it to work for `World`.

Try to implement a `compute_diffusion` method **that wraps around the `diffuse`
free function**, to read the temperatures of the patches and change them
based on the output of `diffuse`.

> :exclamation: All the tests already implemented for diffuse should keep
> working, its interface must, therefore, stay the same.

We will upload and discuss our proposed solution tomorrow.
