#include "patch.hpp"
#include "world.hpp"

int main()
{
  int size{20};
  double start_black_percentage{0.2};
  double start_white_percentage{0.2};
  int max_age{25};
  World world(size, start_black_percentage, start_white_percentage, max_age);
  int iterations{500};
  bool print_to_screen = true;
  if (size > 30) {
    print_to_screen = false;
  }
  simulate(world, iterations, print_to_screen);
  return 0;
}
