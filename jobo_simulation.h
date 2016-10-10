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

private:
  parameters m_parameters;
};

} //~namespace jobo

#endif // JOBO_SIMULATION_H
