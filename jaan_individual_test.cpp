#include <random>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "jaan_individual.h"
#include "jaan_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(jaan_individual_generation_use) {
  Parameters p(10000,   // Max generations
               1000,    // population size
               20,      // number of preference genes
               20,      // number of trait genes
               10,      // Number of quality genes
               0.0,     // optimum preference
               0.0,     // optimum trait
               1,       // value of preference
               0.01,    // value of trait
               1.0e-3,  // Pref and trt mutation rate
               1.0e-4,  // Chance quality gene goes up
               1.0e-2); // Chance quality gene goes down
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
  Parameters p(10000,   // Max generations
               1000,    // population size
               20,      // number of preference genes
               20,      // number of trait genes
               10,      // Number of quality genes
               0.0,     // optimum preference
               0.0,     // optimum trait
               1,       // value of preference
               0.01,    // value of trait
               1.0e-3,  // Pref and trt mutation rate
               1.0e-4,  // Chance quality gene goes up
               1.0e-2); // Chance quality gene goes down
  std::mt19937 generator(42);
  std::vector<Individual> population(p.get_pop_size(), Individual(p, generator));
  for (int k = 0; k < p.get_pop_size(); ++k) {
    population[k].male_viability_function(population, p, generator);
  }
}

#pragma GCC diagnostic pop



