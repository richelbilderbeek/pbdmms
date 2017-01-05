#include "elly_experiment.h"

#include "elly_simulation.h"
#include "daic_run.h"

elly::experiment::experiment(const parameters& p)
  : m_daisie_input_with_main_ext{},
    m_daisie_input_without_main_ext{},
    m_daisie_output_with_main_ext{},
    m_daisie_output_without_main_ext{},
    m_simulation_results{}
{
  //Simulate a species ontogeny and migration pattern
  simulation s(p);
  s.run();
  m_simulation_results = get_results(s);

  //Feed it to DAISIE
  m_daisie_input_with_main_ext = convert_to_daisie_input_with_main_ext(m_simulation_results);
  m_daisie_input_without_main_ext = convert_to_daisie_input_without_main_ext(m_simulation_results);

  const double init_lambda_c{p.get_clado_rate_is().get()}; //Which lambda here, island or mainland?
  const double init_mu{p.get_ext_rate_is().get()}; //Which rate here, island or mainland?
  const int init_k{p.get_carryingcap_is()}; //Which carrying capacity here, island or mainland?
  const double init_gamma{p.get_mig_rate_to_island().get()};
  const double init_lambda_a{p.get_ana_rate().get()};

  m_daisie_output_with_main_ext = daic::run(
    m_daisie_input_with_main_ext,
    init_lambda_c,
    init_mu,
    init_k,
    init_gamma,
    init_lambda_a
  );

  m_daisie_output_without_main_ext = daic::run(
    m_daisie_input_without_main_ext,
    init_lambda_c,
    init_mu,
    init_k,
    init_gamma,
    init_lambda_a
  );
}
