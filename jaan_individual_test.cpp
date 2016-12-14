#include <random>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "jaan_individual.h"
#include "jaan_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(jaan_individual_generation_use) {
  Parameters p;
  std::mt19937 generator(42);
  const Individual i(p, generator);
  const Individual j(i);
  BOOST_CHECK(i == j);
  std::mt19937 generator1(42);
  const Individual k(i, j, p, generator1);
  std::mt19937 generator2(42);
  const Individual l(i, j, p, generator2);
  BOOST_CHECK(k == l);
}

BOOST_AUTO_TEST_CASE(jaan_individual_mateSelect_use) {
  Parameters p;
  std::mt19937 generator(42);
  std::vector<Individual> population(p.get_popSize(), Individual(p, generator));
  for (int k = 0; k < p.get_popSize(); ++k) {
    population[k].mateSelect(population, p, generator);
  }
}

#pragma GCC diagnostic pop



