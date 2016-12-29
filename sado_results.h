#ifndef SADO_RESULTS_H
#define SADO_RESULTS_H

#include <vector>

namespace sado {

class results
{
public:
  results();

  std::vector<std::vector<double>> m_ecological_trait;
  std::vector<std::vector<double>> m_female_preference;
  std::vector<std::vector<double>> m_male_trait;

  //Timepoints that have been measured
  std::vector<int> m_t;

  std::vector<double> m_rhopq;
  std::vector<double> m_rhoxp;
  std::vector<double> m_rhoxq;
  std::vector<double> m_sx;
  std::vector<double> m_sp;
  std::vector<double> m_sq;
};

} //~namespace sado

#endif // SADO_RESULTS_H
