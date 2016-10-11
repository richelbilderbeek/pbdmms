#ifndef JOBO_SIMULATION_H
#define JOBO_SIMULATION_H

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
  individuals m_individuals;
  const parameters m_parameters;
};

} //~namespace jobo

#endif // JOBO_SIMULATION_H
