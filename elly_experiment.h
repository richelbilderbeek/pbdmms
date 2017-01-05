#ifndef ELLY_EXPERIMENT_H
#define ELLY_EXPERIMENT_H

#include "elly_parameters.h"
#include "elly_results.h"
#include "daic_output.h"

namespace elly {

///A full experiment:
/// * supply a set of parameters that will be the truth
/// * with those parameters, run a simulation to create a species ontogeny and migration pattern
/// * feed those results to DAISIE
/// * let DAISIE estimate back the parameters used
class experiment
{
public:
  experiment(const parameters& p);

  const auto& get_simulation_results() const noexcept { return m_simulation_results; }
  const auto& get_daisie_results() const noexcept { return m_daisie_output; }

private:

  daic::output m_daisie_output;
  results m_simulation_results;
};

} //~namespace elly

#endif // ELLY_EXPERIMENT_H
