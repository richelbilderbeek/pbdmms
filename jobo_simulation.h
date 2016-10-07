#ifndef JOBO_SIMULATION_H
#define JOBO_SIMULATION_H

#include "jobo_parameters.h"

class jobo_simulation
{
public:
  jobo_simulation(const jobo_parameters& parameters);

  jobo_parameters get_parameters() { return m_parameters; }

private:
  jobo_parameters m_parameters;
};

#endif // JOBO_SIMULATION_H
