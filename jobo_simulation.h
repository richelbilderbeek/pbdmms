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

private:
  ///Individuals of current generations
  individuals m_individuals;
  const parameters m_parameters;
  std::mt19937 m_rng_engine;
};

std::vector<int> get_random_ints(std::mt19937& rng_engine, int n);
std::vector<double> get_random_doubles(std::mt19937& rng_engine, int n);
std::vector<int> get_random_parents(std::mt19937& rng_engine, int population_size);

  ///Go the next generations
std::vector<individual> goto_next_generation(
  std::vector<individual> individuals,
  const double mutation_rate,
  std::mt19937& rng_engine
);


std::vector<individual> extinction_low_fitness(std::vector<individual> new_individuals);
std::vector<individual> connect_generations(std::vector<individual> individuals, const double mutation_rate, std::mt19937 &rng_engine, int generationss);


} //~namespace jobo

#endif // JOBO_SIMULATION_H
