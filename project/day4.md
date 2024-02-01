# Project session 4 (Thursday)

## Highlights on random number generation

Today we will reach the point where we need to add _(pseudo) randomness_ to the
behavior of some of the `World` class methods.

Let's therefore set some **good practices** about the generating random numbers
in C++.
We will do that, by studying what this _simple example_ is doing, and why
it is written like this:

```c++
#include <iostream>
#include <random>

int main() {
  std::default_random_engine eng{};
  double mean = 2.;
  double sigma = 5.;
  std::normal_distribution<double> gauss{mean, sigma};

  for (int i{0}; i != 10; ++i) {
    std::cout << gauss(eng) << '\n';
  }
}
```

First, by looking at the includes, we can see that the _standard library_
provides tools to handle random number generation in
[`random`](https://en.cppreference.com/w/cpp/header/random).

Second, we can see that we are actually using two distinct components:

- We create a so called _random engine_ (`std::default_random_engine eng{}`):  
  this is what actually defines the pseudo-random generation algorithm we are using;
- We use a template class (`std::normal_distribution<double> gauss{mean, sigma}`):  
  it allows us to generate random numbers according to a specific, and
  configurable (`gauss{mean, sigma}`) statistical distribution.

> **Good practice**: you can generate random numbers according to a wide set of
> statistical distributions, **don't reinvent the wheel** by processing
> yourself results from the _random engine_ directly (unless you _really_ need).
>
> **Good practice**: avoid C _random generation_ functions.

At this point, write the above program in a dedicated file, try to compile it
run it a few times.

> :exclamation: You should see results that are always the same!

This is because the `default_random_engine` generates numbers _pseudo randomly_,
starting always from the same seed.
If you make the _seed_ random, then the program behavior should change every
time, you can do that by applying the following change:

``` c++
  std::random_device r;
  std::default_random_engine eng{r()};
```

In this case, for seeding, we use `std::random_device`:  
 _a (possibly) non-deterministic random number generator (possibly) using
 hardware entropy source_.

:warning: At this point, you might wondering why bothering with a _random
engine_ if `std::random_device` is available.

> **Good practice**: as `std::random_device` accesses hardware resources, it is
> slower than a _pseudo random_ generator.
> Because of that a two-step approach is recommended.

Now you have all information about what lies beyond the implementation of the
simple program above but, **what to do in a program with a more complex
design?**

Our recommendation, _unless you have special needs_, is:

> **Good practice**: work with a single generator object and propagare it _by
> reference_ wherever you need it.
>
> **Good practice**: instead, build distributions in a (more) local scope,
> closer to where you use them.

We conclude this section with a two remarks:

> **Good practice**: **limit as much as possible the inclusion of randomness
> in your tests** (e.g. aim for program designs that favor testing  by isolating
> the non-random parts that can be tested).
>
> **Should I always use `random_device`** :question: It depends, when testing a
> big application that uses _random numbers_ intensively (e.g. a large
> simulation) being able to control the seed used by the generator is a plus,
> but with the approach we have suggested you would simply need an `if` to
> switch between random and fixed seed.

:exclamation: As we have just discussed `std::vector`, try to experiment by
putting the results from the calls to `gauss{mean, sigma}` into a vector,
then make a second loop:

- to print results
- or to try to compute the sample mean and sigma (may be increasing the number
- of generated numbers)
- or try to remove from the vectors all negative numbers
...
