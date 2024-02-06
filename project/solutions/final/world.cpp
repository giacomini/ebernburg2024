#include "world.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

World::World(int size, double start_black_percentage,
             double start_white_percentage, int max_age)
    : size_(size), max_age_{max_age}
{
  if (start_black_percentage < 0 || start_white_percentage < 0) {
    throw std::runtime_error("Start pecentages must be >=0");
  }
  if (start_black_percentage + start_white_percentage > 1.) {
    throw std::runtime_error("Sum of start percentages greater than 1");
  }
  auto const size2{size * size};
  patches_.reserve(size2);

  std::uniform_int_distribution<> flat{0, max_age_};
  // clang-format off
  auto out = std::generate_n(std::back_inserter(patches_), size2 * start_black_percentage,
                             [&]() { return Patch(Daisy::Black, 0., flat(eng_)); });
       out = std::generate_n(out, size2 * start_white_percentage,
                             [&]() { return Patch(Daisy::White, 0., flat(eng_)); });
             std::generate_n(out, size2 - patches_.size(),
                             [&]() { return Patch(Daisy::Barren, 0., flat(eng_)); });
  // clang-format on
  std::shuffle(patches_.begin(), patches_.end(), eng_);
}

void World::spread()
{
  for (auto& patch : patches_) {
    patch.age_and_die(max_age_);
  }
  auto new_patches = patches_;

  std::uniform_real_distribution<double> flat;
  for (int idx{0}; idx < size_ * size_; ++idx) {
    auto& patch = patches_[idx];
    if (patch.daisy() == Daisy::Barren ||
        flat(eng_) >= patch.seeding_threshold()) {
      continue;
    }
    std::vector<Patch*> barren_neighbors;
    int const row = idx / size_;
    int const col = idx % size_;
    for (int neighborRow : {row - 1, row, row + 1}) {
      for (int neighborCol : {col - 1, col, col + 1}) {
        if (neighborRow >= 0 && neighborRow < size_ && neighborCol >= 0 &&
            neighborCol < size_) {
          auto& neighbor = new_patches[neighborRow * size_ + neighborCol];
          if (neighbor.daisy() == Daisy::Barren) {
            barren_neighbors.emplace_back(&neighbor);
          }
        }
      }
    }
    if (barren_neighbors.size() > 0) {
      std::uniform_int_distribution<> flat_i(0, barren_neighbors.size() - 1);
      barren_neighbors[flat_i(eng_)]->sprout(patch.daisy());
    }
  }
  patches_ = new_patches;
}

void World::print()
{
  for (int i{0}; i < size_; ++i) {
    for (int j{0}; j < size_; ++j) {
      std::cout << patches_[i * size_ + j];
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void World::write_to_file(std::ofstream& out_file, double solar_luminosity)
{
  struct Sums
  {
    double global_t{0.};
    int n_white{0};
    int n_black{0};
    int n_barren{0};
  };

  auto sums = std::accumulate(patches_.begin(), patches_.end(), Sums{},
                              [](Sums s, auto const& p) {
                                s.global_t += p.temperature();
                                switch (p.daisy()) {
                                  case Daisy::White:
                                    ++s.n_white;
                                    break;
                                  case Daisy::Black:
                                    ++s.n_black;
                                    break;
                                  case Daisy::Barren:
                                    ++s.n_barren;
                                    break;
                                }
                                return s;
                              });

  sums.global_t /= patches_.size();

  out_file << solar_luminosity << ',' << sums.global_t << ',' << sums.n_black
           << ',' << sums.n_white << ',' << sums.n_barren << '\n';
}

void World::compute_temperatures(double solar_luminosity)
{
  for (auto& patch : patches_) {
    patch.absorb_light(solar_luminosity);
  }
}

void World::compute_diffusion()
{
  std::vector<double> temperatures;
  temperatures.reserve(patches_.size());
  for (auto const& p : patches_) {
    temperatures.push_back(p.temperature());
  }

  auto new_temperatures = diffuse(temperatures, size_, 0.5);

  for (int i{0}; i < size_ * size_; ++i) {
    patches_[i].temperature(new_temperatures[i]);
  }
}

std::vector<double> diffuse(std::vector<double> const& temperatures, int size,
                            double diffusion_rate)
{
  std::vector<double> new_temperatures(size * size);
  for (int idx{0}; idx < size * size; ++idx) {
    double temperature = temperatures[idx];
    double const diffused_temperature = temperature * diffusion_rate / 8;
    int const row = idx / size;
    int const col = idx % size;
    for (int neighborRow : {row - 1, row, row + 1}) {
      for (int neighborCol : {col - 1, col, col + 1}) {
        // Check if the neighbor is within the grid bounds
        if (neighborRow >= 0 && neighborRow < size && neighborCol >= 0 &&
            neighborCol < size) {
          new_temperatures[neighborRow * size + neighborCol] +=
              diffused_temperature;
          temperature -= diffused_temperature;
        }
      }
    }
    // Keep leftover temperature
    new_temperatures[idx] += temperature;
  }
  // Update original patches
  return new_temperatures;
}

void World::step(double solar_luminosity)
{
  compute_temperatures(solar_luminosity);
  compute_diffusion();
  spread();
}

void simulate(World world, int iterations, bool print_to_screen)
{
  int it{0};
  double solar_luminosity(1);
  std::ofstream out_file("data.csv");
  out_file << "Solar luminosity, Global temperature, Black daisies, White "
              "daisies, Barren lands\n";
  while (it++ < iterations) {
    world.step(solar_luminosity);
    std::cout << "\033c";
    if (print_to_screen) {
      world.print();
    }
    world.write_to_file(out_file, solar_luminosity);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  out_file.close();
}
