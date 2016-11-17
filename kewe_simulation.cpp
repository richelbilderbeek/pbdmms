#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_random.h"

simulation::simulation(const kewe_parameters& parameters)
  : m_parameters{parameters},
    m_generator(parameters.sim_parameters.seed),
    m_results{},
    m_output{}
{

}

void simulation::run()
{
  SetSeed(get_parameters().sim_parameters.seed);
  create_header(get_parameters());
  std::vector<indiv> pop = create_initial_population(get_parameters());

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;

  iterate(histX, histP, histQ, get_parameters(), pop, m_output);

  outputLTT(histX, histP, histQ, get_parameters());

  m_results.m_ecological_trait = histX;
  m_results.m_female_preference = histP;
  m_results.m_male_trait = histQ;
}
