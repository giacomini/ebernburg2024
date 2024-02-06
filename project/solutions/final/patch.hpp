#if !defined(PATCH_H)
#define PATCH_H

#include <cmath>
#include <iostream>

enum class Daisy
{
  Black,
  White,
  Barren
};

class Patch
{
  Daisy daisy_{Daisy::Barren};
  double temperature_{0.};
  int age_{0};
  struct Albedo
  {
    static constexpr double black = 0.25;
    static constexpr double white = 0.75;
    static constexpr double surface = 0.4;
  };

 public:
  Patch() = default;
  Patch(Daisy daisy, double temperature, int age)
      : daisy_(daisy), temperature_(temperature), age_(age)
  {
    if (age < 0) {
      throw std::runtime_error("Age is negative");
    }
  }

  double temperature() const
  {
    return temperature_;
  }
  void temperature(double temperature)
  {
    temperature_ = temperature;
  }

  friend std::ostream& operator<<(std::ostream& os, const Patch& patch);

  void absorb_light(double solar_luminosity);

  void age_and_die(int max_age);

  double seeding_threshold()
  {
    return ((0.1457 * temperature_) - (0.0032 * (temperature_ * temperature_)) -
            0.6443);
  }

  void sprout(Daisy daisy);

  Daisy daisy() const
  {
    return daisy_;
  };
};

inline double local_heating(double solar_luminosity, double albedo)
{
  double absorbed_luminosity{(1 - albedo) * solar_luminosity};
  return absorbed_luminosity > 0 ? 72. * std::log(absorbed_luminosity) + 80.
                                 : 80.;
}

#endif  // PATCH_H
