#include "elly_experiment.h"

#include "elly_simulation.h"
#include "daic_run.h"

elly::experiment::experiment(
  const parameters& p
) : m_input_ideal{},
    m_input_reality{},
    m_output_ideal{},
    m_output_reality{},
    m_parameters{p},
    m_sim_measurements{},
    m_sim_results{}
{
}

void elly::experiment::create_daisie_input()
{
  //Feed it to DAISIE
  m_input_ideal = convert_ideal(m_sim_results);
  m_input_reality = convert_reality(m_sim_results);
}

void elly::experiment::run()
{
  run_sim();
  create_daisie_input();
  run_daisie();
}

void elly::experiment::run_daisie()
{
  const auto& p = m_parameters;
  const double init_lambda_c{p.get_clado_is().get()}; //island or mainland?
  const double init_mu{p.get_ext_is().get()}; //island or mainland?
  const int init_k{p.get_carryingcap_is().get().get()}; //island or mainland?
  const double init_gamma{p.get_mig_to_island().get()};
  const double init_lambda_a{p.get_ana_rate().get()};
  const double island_age{p.get_crown_age()};
  const int n_species_main{p.get_init_n_main_sps()};

  //dii: DAISIE input ideal
  //doi: DAISIE output ideal
  //dir: DAISIE input reality
  //dor: DAISIE output reality
  const std::string dii_filename = "experiment_daisie_input_ideal.csv";
  const std::string doi_filename = "experiment_daisie_output_ideal.csv";
  const std::string dir_filename = "experiment_daisie_input_reality.csv";
  const std::string dor_filename = "experiment_daisie_output_reality.csv";

  const std::string r_script_filename = "experiment.r";

  m_output_ideal = daic::run(
    m_input_ideal,
    init_lambda_c,
    init_mu,
    init_k,
    init_gamma,
    init_lambda_a,
    island_age,
    n_species_main,
    dii_filename,
    doi_filename,
    r_script_filename
  );

  m_output_reality = daic::run(
    m_input_reality,
    init_lambda_c,
    init_mu,
    init_k,
    init_gamma,
    init_lambda_a,
    island_age,
    n_species_main,
    dir_filename,
    dor_filename,
    r_script_filename
  );

}

void elly::experiment::run_sim()
{
  simulation s(m_parameters);
  s.run();
  m_sim_results = get_results(s);
  m_sim_measurements = s.get_measurements();
}
