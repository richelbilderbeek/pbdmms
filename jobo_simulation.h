#ifndef JOBO_SIMULATION_H
#define JOBO_SIMULATION_H

#include <random>
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"

namespace jobo {

class simulation

{
public:
  simulation(const parameters& parameters) noexcept;
  parameters get_parameters() const noexcept { return m_parameters;}
  individuals get_individuals() const noexcept { return m_individuals;}
  ///Go the next generation
  void goto_next_generation();

private:
  ///Individuals of current generation
  individuals m_individuals;
  const parameters m_parameters;
  std::mt19937 m_rng_engine;
};

/*
class new_individual
{
public:
new_individual(
    const std::string& genotype = ""
  );

private:
  genotype m_genotype;
};
*/

std::vector<int> get_random_ints(std::mt19937& rng_engine, int n);
std::vector<double> get_random_doubles(std::mt19937& rng_engine, int n);
std::vector<int> get_random_parents(std::mt19937& rng_engine, int population_size);

  ///Go the next generation
std::vector<individual> goto_next_generation(
  std::vector<individual> individuals,
  //genotype,
  //const double mutation_rate,
  std::mt19937& rng_engine
);

//using new_individuals = std::vector<new_individual>;

} //~namespace jobo

#endif // JOBO_SIMULATION_H
