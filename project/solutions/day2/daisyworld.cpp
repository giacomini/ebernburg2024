#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cmath>

#include "doctest.h"

double local_heating(double solar_luminosity, double albedo) {
  double absorbed_luminosity{(1 - albedo) * solar_luminosity};
  double local_heating =
      absorbed_luminosity > 0 ? 72. * std::log(absorbed_luminosity) + 80. : 80.;
  return local_heating;
}

std::vector<double> diffuse(std::vector<double> const& temperatures, int size,
                            double diffusion_rate) {
  std::vector<double> new_temperatures(size * size);
  for (int idx{0}; idx < size * size; ++idx) {
    double temperature = temperatures[idx];
    double const diffused_temperature = temperature * diffusion_rate / 8.;
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

auto sum_all(std::vector<double> const& v) {
  double sum{0.};
  for (auto& e : v) {
    sum += e;
  }
  return sum;
}

TEST_CASE("Testing local heating") {
  double solar_luminosity = 1.;
  CHECK(local_heating(solar_luminosity, 1.) ==
        doctest::Approx(80.).epsilon(0.1));
  CHECK(local_heating(solar_luminosity, 0.) ==
        doctest::Approx(80.).epsilon(0.1));
  CHECK(local_heating(solar_luminosity, 0.5) ==
        doctest::Approx(30.).epsilon(0.1));
  CHECK(local_heating(solar_luminosity, 0.99) ==
        doctest::Approx(-252.).epsilon(0.1));
}

TEST_CASE("Testing diffusion") {
  SUBCASE("Uniform temperature") {
    SUBCASE("Temperature=0") {
      double diffusion_rate = 1.;
      double temperature = 0.;
      std::vector<double> temperatures(9, temperature);
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) ==
            doctest::Approx(sum_all(temperatures)));
      for (auto const& t : diffused_temperature) {
        CHECK(t == doctest::Approx(0.));
      }
    }

    SUBCASE("Temperature=8") {
      double diffusion_rate = 1.;
      double temperature = 8.;
      std::vector<double> temperatures(9, temperature);
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) ==
            doctest::Approx(sum_all(temperatures)));
      for (auto const& t : diffused_temperature) {
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
