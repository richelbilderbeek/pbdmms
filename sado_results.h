#ifndef SADO_RESULTS_H
#define SADO_RESULTS_H

#include "sado_fwd.h"
#include "sado_histogram.h"
#include "sado_histograms.h"
#include "sado_parameters.h"
#include "sado_result.h"
#include "sado_species.h"
#include <iosfwd>
#include <string>
#include <vector>

namespace sado
{

class results
{
public:
  results(const parameters& p);

  void add_result(const result& r) { m_results.push_back(r); }
  void add_species(const species& sp) { m_spp.push_back(sp); }
  void add_species(const std::vector<species>& v);

  std::vector<int> collect_ts() const noexcept;
  std::vector<double> collect_rhopqs() const noexcept;
  std::vector<double> collect_rhoxps() const noexcept;
  std::vector<double> collect_rhoxqs() const noexcept;
  std::vector<double> collect_sxs() const noexcept;
  std::vector<double> collect_sps() const noexcept;
  std::vector<double> collect_sqs() const noexcept;

  const auto& get_results() const noexcept { return m_results; }
  const auto& get_species() const noexcept {return m_spp;}
  auto size() const noexcept { return m_results.size(); }

private:
  std::vector<result> m_results;
  std::vector<species> m_spp;

  const parameters m_p;

  friend std::ostream &operator<<(std::ostream &os, const results &r) noexcept;
};

/// Gather all ecological traits' histograms
histograms collect_ecological_traits(const results& r) noexcept;

/// Gather all female preferences' histograms
histograms collect_female_preferences(const results& r) noexcept;

/// Gather all male sexual traits' histograms
histograms collect_male_traits(const results& r) noexcept;

void create_header(const parameters& p);

std::string create_header_str(const parameters& p);

/// This is the output created by the first version of the program
std::vector<std::string> get_golden_output() noexcept;

std::ostream &operator<<(std::ostream &os, const results &r) noexcept;

} //~namespace sado

#endif // SADO_RESULTS_H
