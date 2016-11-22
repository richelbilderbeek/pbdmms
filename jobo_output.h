#ifndef JOBO_OUTPUT_H
#define JOBO_OUTPUT_H

#include <set>
#include <random>
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_simulation.h"
#include "jobo_results.h"
#include <stdexcept>

using namespace std;

namespace jobo {

///Run simulation and obtains a vector of nLTT values of good species
vector<int> run_simulation(
  const parameters& parameters,
  std::vector<individual> individuals
);

/*
///?What is the purpose of this function?
vector<int> get_m_ltt_good(
  int time,
  const double mutation_rate,
  std::mt19937& rng_engine,
  std::vector<individual>individuals
);
*/

} //~namespace jobo
#endif // JOBO_OUTPUT_H
