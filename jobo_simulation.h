#ifndef JOBO_SIMULATION_H
#define JOBO_SIMULATION_H

#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"

namespace jobo {

class simulation
{
public:
  simulation(const parameters& parameters);
  parameters get_parameters() const { return m_parameters;}

  simulation(const individuals& individuals);
  individuals get_individuals() const {return m_individual;}

private:
  parameters m_parameters;
  individuals m_individual;
};

} //~namespace jobo

#endif // JOBO_SIMULATION_H
