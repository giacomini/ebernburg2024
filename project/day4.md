# Project session 4 (Thursday)

- [Project session 4 (Thursday)](#project-session-4-thursday)
  - [`World`'s member functions with a stochastic behavior](#worlds-member-functions-with-a-stochastic-behavior)
    - [Highlights on random number generation](#highlights-on-random-number-generation)
    - [Randomizing the initial state](#randomizing-the-initial-state)
    - [Spreading daisies](#spreading-daisies)
  - [Write to file the status of the simulation](#write-to-file-the-status-of-the-simulation)
    - [Optional (part 1)](#optional-part-1)
  - [Running the simulation](#running-the-simulation)
    - [Optional (part 2)](#optional-part-2)

You are **almost there**!
Let's **complete a few parts** of the project profiting of the new
**functionalities introduced today**.

## `World`'s member functions with a stochastic behavior

### Highlights on random number generation

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

- to print results;
- or to try to compute the sample mean and sigma (may be increasing the number
  of generated numbers);
- or try to remove from the vectors all negative numbers;
- ...

### Randomizing the initial state

One thing we have mentioned yesterday, was that the `World(int, double, double)`
constructor was not populating the grid by positioning patches in a random
position (e.g. _white daisies where one close to the other_).

Moreover we also noticed that we wanted to generate an initial state where
daises have a random initial age within the integer range [`0`, `max_age`).

Let's now try to **add randomness to that constructor**, by starting with a few
considerations:

- in our case, we don't need the _random engine_ outside `World` (plus we don't
  have implemented a `main` yet), we can agree that the _random engine_
  will be one of the `World` private member variables;
- instead we would suggest that the distribution we need can safely live in the
  scope of the constructor;
- when it comes to randomizing the position of the patches in the grid, we
  suggest to initially fill an ordered grid, and then use the
  [`std::shuffle`](https://en.cppreference.com/w/cpp/algorithm/random_shuffle)
  algorithm to randomize `Patch` positions.

> :tada: Once done with this assignment, you should start understanding why we
> decided to represent a 2D grid with a single `std::vector`, among other
> things, we can easily profit of _algorithms_!
>
> :exclamation: The brave ones can experiment with the
> [`std::generate_n`](https://en.cppreference.com/w/cpp/algorithm/generate_n)
> algorithm to initially fill the `patches_` vector.

### Spreading daisies

The next function in our code that will make use of the random generation is the
`spread` function.

This function should be a method of `World`, that deals with the spread of
flowers to neighboring patches under specific conditions:

- a flower patch will spread its seeds only if it's still alive;
- the probability of spreading seeds will vary based on the local temperature;
- the seed will spread to a single random barren neighboring patch.

For the first point, you should make use of the `check_survival` function, that
will _age_ and (potentially) _kill_ the daisies in the patch of land.

For the second you will need to implement a new method within the `Patch` class.
This method will return the probability of the patch to seed the neighboring
patches based on this equation:

$$ f(t) = -0.6443 + 0.1457\; T - 0.0032\; T^2 $$

> **NOTE**: in our case the new `Patch` method was called `seeding_threshold`.

Then, within `World`, you should use again a random distribution to decide if
the flowers will spread or not.

If both checks pass (the flower is still alive after aging and it
passed the random roll to spread) you will need to check its neighbors for a
_barren land_ to spread to.

You should use a method similar to the one from the `diffuse` function to
iterate through the neighbors. If you find a `Barren` neighbor you should add it
to a list of available patches.

You will then randomly choose one element of such list to sprout a seed
of the same type of daisy of the current patch.

> :exclamation: Since you will need to call the `sprout` function of the
> neighbor, you cannot use a copy of the neighbor. How would you do it?

<details><summary>Discover our solution</summary>

Instead of creating a list of barren neighbors, you could instead create a list
of indexes to the same neighbors.

Or you can define the list as `std::vector<Patch*> barren_neighbors;` use the
`emplace_back` method of vector to fill the neighbor's list it
with the `address` of the neighbors. This will allow us to call the `sprout`
method with `->` access operator: `barren_neighbors[...]->sprout(...)`.

</details>

As per diffusion, we will need to use a new copy of the `patches_` vector, since
we don't want a newly born flower to spread as soon as it appears.

:exclamation: Based on the previous description **try to implement `spread`**.

## Write to file the status of the simulation

Let's start outputting the results of the simulation.
To do that we need to **create a function that will write to file some values**.

In particular, we would like to have saved to file:

- the solar luminosity;
- the global average temperature of our world;
- the number of black daisies;
- the number of white daisies;
- the number of barren patches of land.

Create a `write_to_file` method for `World` with the following interface:

```c++
void write_to_file(std::ofstream &out_file, double solar_luminosity)
```

> :exclamation: The stream to the output file is accessed by reference from the
> arguments.
> :exclamation: You can write to the `out_file` stream exactly as you would do
> with `std::cout`, using the `<<` operator.

The solar luminosity is passed as an argument, since we should allow it to change
every iteration of the simulation, as such is not a member of `World`.

The average temperature of the world can be computed by using the
`std::accumulate` function.

The population counts can be implemented using the `std::count_if` algorithm.

:exclamation: Try to implement the `write_to_file` function as described above.

### Optional (part 1)

If you have time, you can try to implement sums and counts in `write_to_file`
in a single loop by using
[`std::accumulate`](https://en.cppreference.com/w/cpp/algorithm/accumulate).

Before trying, please note two things:

- you can define a `struct` also within a function's scope;
- more important, `std::accumulate` **can accumulate also on user defined types**!

Therefore, you could build a `strcut` to be used for the accumulation, as
suggested in this example you should complete:

```c++
void write_to_file(std::ofstream& out_file, double solar_luminosity) {
  struct Sums {
    double global_t{0.};
    int n_white{0};
    int n_black{0};
    int n_barren{0};
  };

  auto sums = std::accumulate(patches_.begin(), patches_.end(), Sums{},
                              []( ... , auto const& p) { ... });
  
  ...
} 
```

## Running the simulation

Now **we have most of the pieces of our model**!

We can hence create a steering function that can be called to run the
simulation.

For simplicity, we will provide you the function itself, which is a method
of the class `World`:

```c++
void simulate(int iterations) {
  int it{0};
  double solar_luminosity(1);
  std::ofstream out_file("data.csv");
  out_file << "Solar luminosity, Global temperature, Black daisies, White "
              "daisies, Barren lands\n";
  while (it++ < iterations) {
    for (auto& patch : patches_) {
      patch.absorb_light(solar_luminosity);
    }
    compute_diffusion();
    spread();
    write_to_file(out_file, solar_luminosity);
  }
  out_file.close();
}
```

In this example the solar luminosity is constant.
The function itself creates the output file to write to, and fills the
first line with the column headers.

The loop runs for the number of iterations given as argument, it could also have
been written with a `for` loop.

Inside the loop we will call the `absorb_light` function for all the patches,
compute the diffusion and spread the daisies.
After all the processing, a new line is written to the output file.

We can try to run this simulation by adding a call to this function to one
`TEST_CASE`.

It's difficult to test a program with stochastic behavior, so we will not discuss
a possible implementation of the tests.

> :exclamation: Still, make sure that there are no compilation error, and the
> `data.csv` file is indeed created!

### Optional (part 2)

If you have time, here are a couple of optional assignments:

> :warning: The only part of the function that cannot be executed from
> "outside" the class is the loop over the patches. How would you modify the
> function and the class to make `simulate` a free function?
>
> :warning: Consider a different implementation of the function, with a free
> `simulate` function that in the loop calls a public method `step` of World
> that performs a single step of the iteration.
> In this implementation do you need all the other methods as `public`?
