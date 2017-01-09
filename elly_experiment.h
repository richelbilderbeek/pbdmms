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

  ///Step 2/4
  void create_daisie_input();

  ///Get the simulation results
  ///Will give something after having called run_sim
  const auto& get_simulation_results() const noexcept
  {
    return m_simulation_results;
  }

  ///Get DAISIE input with mainland extinction
  ///Will give something after having called 'create_daisie_input'
  const auto& get_di_with_main_ext() const noexcept
  {
    return m_input_with_main_ext;
  }
  ///Get DAISIE input without mainland extinction
  ///Will give something after having called 'create_daisie_input'
  const auto& get_di_without_main_ext() const noexcept
  {
    return m_input_without_main_ext;
  }

  ///Step 4/4: Get DAISIE output with mainland extinction
  const auto& get_do_with_main_ext() const noexcept
  {
    return m_output_with_main_ext;
  }
  ///Step 4/4: Get DAISIE output without mainland extinction
  const auto& get_do_without_main_ext() const noexcept
  {
    return m_output_without_main_ext;
  }

  ///Just run all steps
  void run();

  ///Step 3/4
  void run_daisie();

  ///Step 1/4
  void run_sim();

private:

  daic::input  m_input_with_main_ext;
  daic::input  m_input_without_main_ext;
  daic::output m_output_with_main_ext;
  daic::output m_output_without_main_ext;
  const parameters m_parameters;
  results m_simulation_results;
};

} //~namespace elly

#endif // ELLY_EXPERIMENT_H
