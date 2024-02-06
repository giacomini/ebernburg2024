#include "patch.hpp"

#include <cassert>

std::ostream& operator<<(std::ostream& os, const Patch& patch)
{
  switch (patch.daisy_) {
    case Daisy::Black:
      os << "🌸";
      break;
    case Daisy::White:
      os << "💮";
      break;
    case Daisy::Barren:
    default:
      os << "  ";
      break;
  }
  return os;
}

void Patch::sprout(Daisy daisy)
{
  daisy_ = daisy;
  age_ = 0;
}

void Patch::absorb_light(double solar_luminosity)
{
  double albedo{0.};
  switch (daisy_) {
    case Daisy::Black:
      albedo = Albedo::black;
      break;
    case Daisy::White:
      albedo = Albedo::white;
      break;
    case Daisy::Barren:
    default:
      albedo = Albedo::surface;
      break;
  }
  temperature_ = (temperature_ + local_heating(solar_luminosity, albedo)) * 0.5;
}

void Patch::age_and_die(int max_age)
{
  if (++age_ > max_age) {
    daisy_ = Daisy::Barren;
  }
}
