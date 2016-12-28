#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include <string>
#include <vector>
#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_population.h"

namespace sado {

class simulation
{
public:
  simulation(const parameters& p);

  ///Run until the end
  void run();

  ///Do one timestep
  void do_timestep();

  private:
  const parameters m_p;
  population m_pop;

  ///The current generation
  int m_timestep;
};
///Calculate the total competetition an
///individual with ecological trait x
///faces in a population p. It is assumed
///this individual is within that population,
///as this is taken into account: it does not
///suffer from competition with itself
double calc_comp(
  const population& pop,
  const double xi,
  const parameters& p
) noexcept;

///@param as: attractivenesses
offspring create_kids(
  const population& pop,
  const indiv& mother,
  const std::vector<double>& as,
  const parameters& p
);

///May return zero kids
offspring try_to_create_kids(
  const population& pop,
  const int index,
  const parameters& p
);

population create_initial_population(
  const parameters& p
);

void iterate(population pop, const parameters& p);

void kill_mother(const int index, population& pop, const parameters& p);

std::vector<double> get_attractivenesses(
  const population& pop,
  const double pi,
  const double xi,
  const parameters& p
);

} //~namespace sado

#endif // SADO_SIMULATION_H
