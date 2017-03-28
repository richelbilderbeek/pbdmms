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


  std::vector<int> collect_ts() const noexcept;
  std::vector<double> collect_rhopqs() const noexcept;
  std::vector<double> collect_rhoxps() const noexcept;
  std::vector<double> collect_rhoxqs() const noexcept;
  std::vector<double> collect_sxs() const noexcept;
  std::vector<double> collect_sps() const noexcept;
  std::vector<double> collect_sqs() const noexcept;

  const auto& get_results() const noexcept { return m_results; }

  auto size() const noexcept { return m_results.size(); }

private:
  std::vector<result> m_results;

  const parameters m_p;

  friend std::ostream &operator<<(std::ostream &os, const results &r) noexcept;
};

/// Gather all ecological traits' histograms
histograms collect_ecological_traits(const results& r) noexcept;

/// Gather all female preferences' histograms
histograms collect_female_preferences(const results& r) noexcept;

/// Gather all male sexual traits' histograms
histograms collect_male_traits(const results& r) noexcept;

/// Just put all species in one std::vector
/// Expensive to do, information is lost
/// Will be deprecated one day
std::vector<species> collect_species(const results& r) noexcept;

/// Counts the number of all species in all results
int count_n_species(const results& r);

void create_header(const parameters& p);

std::string create_header_str(const parameters& p);

/// This is the output created by the first version of the program
std::vector<std::string> get_golden_output() noexcept;

std::ostream &operator<<(std::ostream &os, const results &r) noexcept;

} //~namespace sado

#endif // SADO_RESULTS_H
