#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_random.h"

simulation::simulation(const kewe_parameters& parameters)
  : m_parameters{parameters},
    m_generator(parameters.sim_parameters.seed),
    m_results{},
    m_output{},
    m_pop{}
{
  create_header(parameters);
  SetSeed(parameters.sim_parameters.seed);
  m_pop = create_initial_population(parameters);
}

void simulation::run()
{
  kewe_parameters parameters = get_parameters();
  create_header(parameters);

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;
  result_variables output_variables;
  std::vector<indiv> pop = create_initial_population(parameters);

  for (unsigned int t = 0; t < parameters.sim_parameters.endtime; ++t)
    {
      pop = create_next_generation(parameters, pop);
      if(t%parameters.output_parameters.outputfreq==0) // Output once every outputfreq
        output(t, histX, histP, histQ, parameters, pop, output_variables);
    }

  outputLTT(histX, histP, histQ, parameters);

  m_results.m_ecological_trait = histX;
  m_results.m_female_preference = histP;
  m_results.m_male_trait = histQ;

}
