#ifndef SADO_RESULTS_H
#define SADO_RESULTS_H

#include <iosfwd>
#include <string>
#include <vector>
#include "sado_histogram.h"
#include "sado_histograms.h"
#include "sado_fwd.h"
#include "sado_parameters.h"

namespace sado {

class results
{
public:
  results(const parameters& p);

  histograms m_ecological_trait;
  histograms m_female_preference;
  histograms m_male_trait;

  //Timepoints that have been measured
  std::vector<int> m_t;

  std::vector<double> m_rhopq;
  std::vector<double> m_rhoxp;
  std::vector<double> m_rhoxq;
  std::vector<double> m_sx;
  std::vector<double> m_sp;
  std::vector<double> m_sq;

  const parameters m_p;
};

void create_header(const parameters& p);

std::string create_header_str(const parameters& p);

std::vector<std::string> get_golden_output() noexcept;

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace sado

#endif // SADO_RESULTS_H
