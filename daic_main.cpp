#include <cassert>
#include <iostream>

#include "daic_parameters.h"
#include "daic_simulation.h"
#include "daic_results.h"

using namespace daic;

int main()
{
  const auto p = create_test_parameters();
  simulation s(p);
  s.run();
  std::cout << s.get_results() << '\n';
}
