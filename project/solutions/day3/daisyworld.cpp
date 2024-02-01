#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <cmath>

#include "doctest.h"

enum class Daisy { Black, White, Barren };

double local_heating(double solar_luminosity, double albedo);
std::vector<double> diffuse(std::vector<double> const& temperatures, int size,
                            double diffusion_rate);

class Patch {
  Daisy daisy_{Daisy::Barren};
  double temperature_{0.};
  int age_{0};
  struct Albedo {
    static constexpr double black{0.25};
    static constexpr double white{0.75};
    static constexpr double surface{0.4};
  };

 public:
  Patch() = default;
  Patch(Daisy daisy, double temperature, int age)
      : daisy_(daisy), temperature_(temperature), age_(age) {
    if (age < 0) {
      throw std::runtime_error("Age is negative");
    }
  }

  void sprout(Daisy daisy) {
    daisy_ = daisy;
    age_ = 0;
  }

  Daisy daisy() const { return daisy_; }

  double temperature() const { return temperature_; }
  void temperature(double temperature) { temperature_ = temperature; }

  void absorb_light(double solar_luminosity) {
    double albedo{0.};
    switch (daisy_) {
      case Daisy::Black:
        albedo = Albedo::black;
        break;
      case Daisy::White:
        albedo = Albedo::white;
        break;
      case Daisy::Barren:
        albedo = Albedo::surface;
        break;
    }
    temperature_ =
        (temperature_ + local_heating(solar_luminosity, albedo)) * 0.5;
  }

  bool check_survival(int max_age) {
    if (daisy_ == Daisy::Barren) {
      return false;
    }
    if (++age_ > max_age) {
      daisy_ = Daisy::Barren;
      return false;
    }
    return true;
  }
};

class World {
 private:
  int size_;
  std::vector<Patch> patches_;

 public:
  World(int size, double start_black_perc, double start_white_perc)
      : size_(size), patches_(size * size) {
    for (int i{0}; i < size * size; ++i) {
      if (i < size * size * start_black_perc)
        patches_[i].sprout(Daisy::Black);
      else if (i < size * size * (start_black_perc + start_white_perc))
        patches_[i].sprout(Daisy::White);
      else
        patches_[i].sprout(Daisy::Barren);
    }
  };

  std::vector<Patch> patches() const { return patches_; }

  void compute_diffusion() {
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
};

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

auto sum_all(std::vector<double> const& v) {
  double sum{0.};
  for (auto& e : v) {
    sum += e;
  }
  return sum;
}

TEST_CASE("Test patches") {
  Patch p0(Daisy::Barren, 0., 0);
  Patch p1(Daisy::White, 0., 0);
  Patch p2(Daisy::Black, 10., 25);
  CHECK_THROWS(Patch(Daisy::Barren, 0., -1));

  SUBCASE("Testing construction") {
    CHECK(p0.daisy() == Daisy::Barren);
    CHECK(p1.daisy() == Daisy::White);
    CHECK(p2.daisy() == Daisy::Black);
  }

  SUBCASE("Testing temperature") {
    CHECK(p0.temperature() == 0.);
    CHECK(p1.temperature() == 0.);
    CHECK(p2.temperature() == 10.);
    p1.absorb_light(1.);  // DeltaT = 72 * ln(0.25) +80
    CHECK(p1.temperature() == doctest::Approx(-9.9).epsilon(0.01));
    p2.temperature(20.);
    CHECK(p2.temperature() == 20.);
    p2.absorb_light(1.);  // DeltaT = 72 * ln(0.75) +80
    CHECK(p2.temperature() == doctest::Approx(39.6).epsilon(0.01));
  }

  SUBCASE("Testing age") {
    int max_age = 25;
    CHECK(p0.check_survival(max_age) == false);
    CHECK(p1.check_survival(max_age) == true);
    CHECK(p2.check_survival(max_age) == false);
    CHECK(p2.daisy() == Daisy::Barren);
  }
}

TEST_CASE("Test World") {
  World world1(4, 0.5, 0.25);
  auto wpatches1{world1.patches()};
  World world2(3, 1., 0.);
  auto wpatches2{world2.patches()};
  World world3(3, 0., 0.);
  auto wpatches3{world3.patches()};
  int count_black{0};
  int count_white{0};
  int count_barren{0};
  SUBCASE("World 1 population count") {
    for (auto const& p : wpatches1) {
      if (p.daisy() == Daisy::Black) ++count_black;
      if (p.daisy() == Daisy::White) ++count_white;
      if (p.daisy() == Daisy::Barren) ++count_barren;
    }
    CHECK(count_black == 8);
    CHECK(count_white == 4);
    CHECK(count_barren == 4);
  }
  SUBCASE("World 2 population count") {
    for (auto const& p : wpatches2) {
      if (p.daisy() == Daisy::Black) ++count_black;
      if (p.daisy() == Daisy::White) ++count_white;
      if (p.daisy() == Daisy::Barren) ++count_barren;
    }
    CHECK(count_black == 9);
    CHECK(count_white == 0);
    CHECK(count_barren == 0);
  }
  SUBCASE("World 3 population count") {
    for (auto const& p : wpatches3) {
      if (p.daisy() == Daisy::Black) ++count_black;
      if (p.daisy() == Daisy::White) ++count_white;
      if (p.daisy() == Daisy::Barren) ++count_barren;
    }
    CHECK(count_black == 0);
    CHECK(count_white == 0);
    CHECK(count_barren == 9);
  }
}


TEST_CASE("Testing local heating") {
  double solar_luminosity = 1.;
  SUBCASE("fallback for log(<=0)") {
    CHECK(local_heating(solar_luminosity, 1.) ==
          doctest::Approx(80.).epsilon(0.1));
  }
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
  SUBCASE("Multiple temperature diffusion") {
    SUBCASE("Diffusion rate 1") {
      double diffusion_rate = 1.;
      std::vector<double> temperatures{8., 8., 0., 0., 0., 0., 0., 0., 0.};
      auto diffused_temperature = diffuse(temperatures, 3, diffusion_rate);
      CHECK(sum_all(diffused_temperature) == doctest::Approx(16.));
      CHECK(diffused_temperature[0] == doctest::Approx(6.));
      CHECK(diffused_temperature[1] == doctest::Approx(4.));
      CHECK(diffused_temperature[2] == doctest::Approx(1.));
      CHECK(diffused_temperature[3] == doctest::Approx(2.));
      CHECK(diffused_temperature[4] == doctest::Approx(2.));
      CHECK(diffused_temperature[5] == doctest::Approx(1.));
      CHECK(diffused_temperature[6] == doctest::Approx(0.));
      CHECK(diffused_temperature[7] == doctest::Approx(0.));
      CHECK(diffused_temperature[8] == doctest::Approx(0.));
    }
  }
}
