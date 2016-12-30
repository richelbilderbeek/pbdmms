#ifndef DAIC_SIMULATION_H
#define DAIC_SIMULATION_H

#include "daic_parameters.h"
#include "daic_results.h"

namespace daic {

class simulation
{
public:
  simulation(const parameters& p);
  void run();

  const auto& get_results() const noexcept { return m_results; }

private:
  results m_results;
};

} //~namespace daic

#endif // DAIC_SIMULATION_H
