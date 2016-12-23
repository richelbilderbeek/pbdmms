#include "kewe_simulation.h"
#include "kewe_ses.h"

kewe::simulation::simulation(const parameters& parameters)
  : m_parameters{parameters},
    m_generator(parameters.m_sim_parameters.seed),
    m_results{},
    m_output{},
    m_pop{},
    m_number_generations{0},
    m_ltt_plot{}
{
  create_header(parameters);

  reserve_space_output_vectors(
    m_output,
    m_results.m_ecological_trait,
    m_results.m_female_preference,
    m_results.m_male_trait,
    m_parameters
  );

  m_pop = create_initial_population(parameters.m_sim_parameters, m_generator);

  if (!m_parameters.m_output_parameters.is_silent)
  {
    create_header(m_parameters);
  }
}

void kewe::simulation::run()
{
  const parameters p = get_parameters();

  const int t_end{p.m_sim_parameters.get_end_time()};
  for (int t = 0; t != t_end; ++t)
  {
    m_pop = create_next_generation(p.m_sim_parameters, m_pop, get_generator());

    // Output once every outputfreq
    assert(p.m_output_parameters.outputfreq >= 1);
    if(t % p.m_output_parameters.outputfreq == 0
    )
    {
      do_measurements(
        t,
        m_results.m_ecological_trait,
        m_results.m_female_preference,
        m_results.m_male_trait,
        p,
        m_pop,
        m_output,
        m_ltt_plot
      );
    }
  }
}

void kewe::simulation::reserve_space_output_vectors(
    result_variables& output_variables,
    std::vector<std::vector<double>>& histX,
    std::vector<std::vector<double>>& histP,
    std::vector<std::vector<double>>& histQ,
    const parameters& p)
{
  const int t = p.m_sim_parameters.get_end_time();

  histX.reserve(static_cast<size_t>(t));
  histP.reserve(static_cast<size_t>(t));
  histQ.reserve(static_cast<size_t>(t));

  const int outputfreq = p.m_output_parameters.outputfreq;

  assert(outputfreq > 0);
  assert(t >= 0);
  const int sz{t/outputfreq};
  assert(sz < 1000000);

  output_variables.m_t.reserve(static_cast<size_t>(sz));
  output_variables.m_popsize.reserve(static_cast<size_t>(sz));
  output_variables.m_rhopq.reserve(static_cast<size_t>(sz));
  output_variables.m_rhoxp.reserve(static_cast<size_t>(sz));
  output_variables.m_rhoxq.reserve(static_cast<size_t>(sz));
  output_variables.m_sp.reserve(static_cast<size_t>(sz));
  output_variables.m_sq.reserve(static_cast<size_t>(sz));
  output_variables.m_sx.reserve(static_cast<size_t>(sz));
}

bool kewe::has_bimodal_eco_types(const simulation& s)
{
  return has_bimodal_eco_types(s.get_results());
}

bool kewe::has_branching_mating(const simulation& s)
{
  return has_branching_mating(s.get_result_variables());
}

bool kewe::has_sympatric_speciation(const simulation& s)
{
  return has_sympatric_speciation(s.get_results(), s.get_result_variables());
}
