#include "kewe_simulation.h"
#include "kewe_ses.h"

kewe::simulation::simulation(const parameters& parameters)
  : m_parameters{parameters},
    m_generator(parameters.m_sim_parameters.seed),
    m_results{},
    m_output{},
    m_pop{},
    m_number_generations{0}
{
  create_header(parameters);
  m_pop = create_initial_population(parameters, m_generator);
}

void kewe::simulation::run()
{
  parameters parameters = get_parameters();

  if (!parameters.m_output_parameters.is_silent)
  {
    create_header(parameters);
  }

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;
  result_variables output_variables;
  std::vector<std::pair<bigint,int>> ltt_plot;

  reserve_space_output_vectors(output_variables, histX, histP, histQ, parameters);

  individuals pop = create_initial_population(parameters, m_generator);

  for (unsigned int t = 0; t != parameters.m_sim_parameters.endtime; ++t)
    {
      pop = create_next_generation(parameters, pop, get_generator());

      // Output once every outputfreq
      assert(parameters.m_output_parameters.outputfreq >= 1);
      if(t % parameters.m_output_parameters.outputfreq == 0
      )
      {
        output(t, histX, histP, histQ, parameters, pop, output_variables, ltt_plot);
      }
    }

  //outputLTT(histX, histP, histQ, parameters);

  m_results.m_ecological_trait = histX;
  m_results.m_female_preference = histP;
  m_results.m_male_trait = histQ;

}

void kewe::simulation::reserve_space_output_vectors(
    result_variables& output_variables,
    std::vector<std::vector<double>>& histX,
    std::vector<std::vector<double>>& histP,
    std::vector<std::vector<double>>& histQ,
    const parameters& p)
{
  int t = p.m_sim_parameters.endtime;

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
