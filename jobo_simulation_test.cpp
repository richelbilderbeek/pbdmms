#include "jobo_simulation_test.h"
#include "jobo_individual_test.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"

using namespace jobo;

int jobo::simulation_test() noexcept
{
  int n_fails{0};

  // Random ints are in the supposed range
  {
    const int n_loci{42};
    std::mt19937 rng_engine(42);
    std::vector<int> n_loci_ints = (get_random_ints(rng_engine, n_loci));
    for (int i=0; i!=n_loci; i+=1)
      {
      if (n_loci_ints[i] < 0) ++n_fails;
      if (n_loci_ints[i] > 100) ++n_fails;
      }
  }

  // Random doubles are in the supposed range
  {
    const int n_loci{42};
    std::mt19937 rng_engine(42);
    std::vector<double> n_loci_doubles = (get_random_doubles(rng_engine, n_loci));
    for (int i=0; i!=n_loci; i+=1)
      {
      if (n_loci_doubles[i] < 0) ++n_fails;
      if (n_loci_doubles[i] > 1) ++n_fails;
      }
  }

  //Test get_random_parent
  {
  std::mt19937 rng_engine(42);
  const int population_size(10);
  const int number_of_parents = population_size*2;
  std::vector<int> get_random_parents = (get_random_parent(rng_engine, population_size));
  for (int i=0; i!=number_of_parents; i+=1)
      {
      if (get_random_parents[i] < 1) ++n_fails;
      if (get_random_parents[i] > population_size) ++n_fails;
      }
  }
  return n_fails;
}
