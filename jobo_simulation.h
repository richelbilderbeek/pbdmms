#ifndef JOBO_SIMULATION_H
#define JOBO_SIMULATION_H

#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"

class jobo_simulation
{
public:
  jobo_simulation(const jobo_parameters& parameters);
  jobo_parameters get_parameters() const { return m_parameters;}

  jobo_simulation(const jobo_individuals& individuals);
  jobo_individuals get_individuals() const {return m_individual;}

private:
  jobo_parameters m_parameters;
  jobo_individuals m_individual;
};

#endif // JOBO_SIMULATION_H
