#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_population.h"
#include "sado_results.h"
#include <string>
#include <vector>

namespace sado
{

class simulation
{
public:
  explicit simulation(const parameters &p);

  /// Run until the end
  void run();

  /// Do one timestep
  void do_timestep();

  const auto &get_results() const noexcept { return m_results; }

private:
  /// The parameters used
  const parameters m_parameters;

  /// The current population
  population m_population;

  results m_results;

  /// The current generation
  int m_timestep;
};

/// Calculate the total competetition an
/// individual with ecological trait x
/// faces in a population p. It is assumed
/// this individual is within that population,
/// as this is taken into account: it does not
/// suffer from competition with itself
double
calc_comp(const population &pop, const double xi, const parameters &p) noexcept;

///@param as: attractivenesses
///@return a pair of the offsping created and the father
std::vector<std::pair<indiv, indiv>> create_kids(
    const population &pop,
    const indiv &mother,
    const std::vector<double> &as,
    const parameters &p);

population create_next_generation(const population &pop, const parameters &p);

/// Create the next generation, allow for overlapping generations
population
create_next_generation_overlapping(population pop, const parameters &p);

/// Create the next generation, allow for seperate generations
population
create_next_generation_seperate(const population &pop, const parameters &p);

/// May return zero kids
std::vector<std::pair<indiv, indiv>>
try_to_create_kids(const population &pop, const int index, const parameters &p);

population create_initial_population(const parameters &p);

void iterate(population pop, const parameters &p);

void kill_mother(const int index, population &pop, const parameters &p);

std::vector<double> get_attractivenesses(
    const population &pop,
    const double pi,
    const double xi,
    const parameters &p);

} //~namespace sado

#endif // SADO_SIMULATION_H
