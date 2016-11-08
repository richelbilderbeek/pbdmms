#include "kewe_simulation.h"
#include "kewe_SES.h"

simulation::simulation()
{

}

void simulation::run()
{
  const std::string filename("test_kewe_simulation.csv");
  create_test_parameter_file(filename);
  kewe_parameters parameters = readparameters(filename.c_str());
  std::vector<indiv> pop = initialize();

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;

  iterate(histX, histP, histQ, parameters, pop);

  outputLTT(histX, histP, histQ, parameters);

  m_results.m_ecological_trait = histX;
  m_results.m_female_preference = histP;
  m_results.m_male_trait = histQ;
}
