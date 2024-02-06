#if !defined(WORLD_H)
#define WORLD_H
#include "patch.hpp"

#include <fstream>
#include <random>

class World
{
  int size_{0};
  int max_age_{0};
  std::vector<Patch> patches_;
  std::default_random_engine eng_{};

 public:
  World(int size, double start_black_percentage, double start_white_percentage,
        int max_age);

  std::vector<Patch> const& patches() const
  {
    return patches_;
  }

  void spread();

  void compute_diffusion();

  void compute_temperatures(double solar_luminosity);

  void print();

  void step(double solar_luminosity);

  void write_to_file(std::ofstream& outFile, double solar_luminosity);
};

std::vector<double> diffuse(std::vector<double> const& temperatures, int size,
                            double diffusion_rate);

void simulate(World world, int iterations, bool print_to_screen);

#endif  // WORLD_H
