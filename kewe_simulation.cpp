#include "kewe_simulation.h"
#include "kewe_ses.h"
#include "kewe_results.h"

kewe::simulation::simulation(const parameters& parameters)
  : m_gaussers(parameters.m_sim_parameters),
    m_parameters{parameters},
    m_generator(parameters.m_sim_parameters.seed),
    m_results{},
    m_output{},
    m_pop{},
    m_t{0},
    m_ltt_plot{}
{
  assert(is_valid(m_parameters));
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

void kewe::simulation::do_timestep()
{
  set_pop(create_next_generation(m_parameters.m_sim_parameters, m_pop, get_generator()));
}

void kewe::simulation::run()
{
  const int t_end{m_parameters.m_sim_parameters.get_end_time()};
  while(this->get_generation_number() < t_end)
  {
    do_timestep();
  }

  do_measurements();
}

void kewe::simulation::reserve_space_output_vectors(
    result_variables& output_variables,
    std::vector<std::vector<double>>& histX,
    std::vector<std::vector<double>>& histP,
    std::vector<std::vector<double>>& histQ,
    const parameters& p)
{
  const int t = p.m_sim_parameters.get_end_time();
  const int f = p.m_output_parameters.outputfreq;

  assert(t >= 0);
  //n: number of measurements
  const int n{f == 0 ? 1 : t / f};

  histX.reserve(n);
  histP.reserve(n);
  histQ.reserve(n);

  output_variables.m_t.reserve(n);
  output_variables.m_popsize.reserve(n);
  output_variables.m_rhopq.reserve(n);
  output_variables.m_rhoxp.reserve(n);
  output_variables.m_rhoxq.reserve(n);
  output_variables.m_sp.reserve(n);
  output_variables.m_sq.reserve(n);
  output_variables.m_sx.reserve(n);
}

void kewe::simulation::set_pop(const individuals& pop)
{
  if (must_do_measurements(*this))
  {
    do_measurements();
  }

  m_pop = pop;

  ++m_t;
}

void kewe::simulation::do_measurements()
{
  ::kewe::do_measurements(
    m_t,
    m_results.m_ecological_trait,
    m_results.m_female_preference,
    m_results.m_male_trait,
    m_parameters,
    m_pop,
    m_output,
    m_ltt_plot
  );
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

bool kewe::must_do_measurements(const simulation& s)
{
  if (s.get_parameters().m_output_parameters.outputfreq == 0) return false;
  const int t{s.get_generation_number()};
  if(t %  s.get_parameters().m_output_parameters.outputfreq == 0) return true;
  return false;
}
