#ifndef ELLY_EXPERIMENT_H
#define ELLY_EXPERIMENT_H

#include "elly_parameters.h"
#include "elly_results.h"
#include "elly_measurements.h"
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

  ///Step 2/4
  void create_daisie_input();

  ///Get the simulation's measurements
  ///Will give something after having called run_sim
  const auto& get_sim_measurements() const noexcept
  {
    return m_sim_measurements;
  }

  ///Get the simulation's results
  ///Will give something after having called run_sim
  const auto& get_sim_results() const noexcept
  {
    return m_sim_results;
  }

  ///Get DAISIE input with mainland extinction
  ///Will give something after having called 'create_daisie_input'
  const auto& get_input_ideal() const noexcept
  {
    return m_input_ideal;
  }
  ///Get DAISIE input without mainland extinction
  ///Will give something after having called 'create_daisie_input'
  const auto& get_input_reality() const noexcept
  {
    return m_input_reality;
  }

  ///Step 4/4: Get DAISIE output with mainland extinction
  const auto& get_output_ideal() const noexcept
  {
    return m_output_ideal;
  }
  ///Step 4/4: Get DAISIE output without mainland extinction
  const auto& get_output_reality() const noexcept
  {
    return m_output_reality;
  }

  ///Just run all steps
  void run();

  ///Step 3/4
  void run_daisie();

  ///Step 1/4
  void run_sim();

private:
  daic::input  m_input_ideal;
  daic::input  m_input_reality;
  daic::output m_output_ideal;
  daic::output m_output_reality;
  const parameters m_parameters;
  measurements m_sim_measurements;
  results m_sim_results;
};

} //~namespace elly

#endif // ELLY_EXPERIMENT_H
