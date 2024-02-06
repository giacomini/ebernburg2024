#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "patch.hpp"
#include "world.hpp"

auto sum_all(std::vector<double> const& v)
{
  double sum{0.};
  for (auto& e : v) {
    sum += e;
  }
  return sum;
}

TEST_CASE("Test simulation")
{
  World world(3, 0.3, 0.1, 25);
  int number_of_lines = 0;
  std::string line;
  SUBCASE("no iteration")
  {
    simulate(world, 0, false);
    std::ifstream my_file("data.csv");
    while (std::getline(my_file, line)) {
      ++number_of_lines;
    }
    CHECK(number_of_lines == 1);
  }
  SUBCASE("1 iteration")
  {
    simulate(world, 1, false);
    std::ifstream myfile("data.csv");
    while (std::getline(myfile, line)) {
      ++number_of_lines;
    }
    CHECK(number_of_lines == 2);
  }
  SUBCASE("10 iteration")
  {
    simulate(world, 10, false);
    std::ifstream myfile("data.csv");
    while (std::getline(myfile, line)) {
      ++number_of_lines;
    }
    CHECK(number_of_lines == 11);
  }
}

TEST_CASE("Test patches")
{
  Patch p0(Daisy::Barren, 0., 0);
  Patch p1(Daisy::White, 0., 0);
  Patch p2(Daisy::Black, 10., 25);
  CHECK_THROWS(Patch(Daisy::Barren, 0., -1));

  SUBCASE("Testing construction")
  {
    CHECK(p0.daisy() == Daisy::Barren);
    CHECK(p1.daisy() == Daisy::White);
    CHECK(p2.daisy() == Daisy::Black);
  }

  SUBCASE("Testing temperature")
  {
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

  SUBCASE("Testing age")
  {
    int max_age = 25;
    p0.age_and_die(max_age);
    p1.age_and_die(max_age);
    p2.age_and_die(max_age);
    CHECK(p0.daisy() == Daisy::Barren);
    CHECK(p1.daisy() != Daisy::Barren);
    CHECK(p2.daisy() == Daisy::Barren);
  }
}

TEST_CASE("Test World")
{
  World world1(4, 0.5, 0.25, 25);
  auto wpatches1{world1.patches()};
  World world2(3, 1., 0., 25);
  auto wpatches2{world2.patches()};
  World world3(3, 0., 0., 25);
  auto wpatches3{world3.patches()};
  int count_black{0};
  int count_white{0};
  int count_barren{0};
  SUBCASE("Test throws with wrong percentages")
  {
    CHECK_THROWS(World{4, -1, 0, 0});
    CHECK_THROWS(World{4, 0, -1, 0});
    CHECK_THROWS(World{4, 0.51, 0.67, 0});
  }
  SUBCASE("World 1 population count")
  {
    for (auto const& p : wpatches1) {
      if (p.daisy() == Daisy::Black) ++count_black;
      if (p.daisy() == Daisy::White) ++count_white;
      if (p.daisy() == Daisy::Barren) ++count_barren;
    }
    CHECK(count_black == 8);
    CHECK(count_white == 4);
    CHECK(count_barren == 4);
  }
  SUBCASE("World 2 population count")
  {
    for (auto const& p : wpatches2) {
      if (p.daisy() == Daisy::Black) ++count_black;
      if (p.daisy() == Daisy::White) ++count_white;
      if (p.daisy() == Daisy::Barren) ++count_barren;
    }
    CHECK(count_black == 9);
    CHECK(count_white == 0);
    CHECK(count_barren == 0);
  }
  SUBCASE("World 3 population count")
  {
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

TEST_CASE("Testing local heating")
{
  double solar_luminosity = 1.;
  SUBCASE("fallback for log(<=0)")
  {
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

TEST_CASE("Testing diffusion")
{
  SUBCASE("Uniform temperature")
  {
    SUBCASE("Temperature=0")
    {
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

    SUBCASE("Temperature=8")
    {
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
  SUBCASE("Central temperature diffusion")
  {
    SUBCASE("Diffusion rate 1")
    {
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
    SUBCASE("Diffusion rate 0.5")
    {
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
    SUBCASE("Diffusion rate 0.")
    {
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
  SUBCASE("Corner temperature diffusion")
  {
    SUBCASE("Diffusion rate 1")
    {
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
  SUBCASE("Multiple temperature diffusion")
  {
    SUBCASE("Diffusion rate 1")
    {
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
