#ifndef JOBO_RESULTS_H
#define JOBO_RESULTS_H

#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <stdexcept>
#include <vector>

using namespace std;
namespace jobo {

class results
{
public:
  results();

  /// Add a number of viable lineages for the LTT plot
  void add_ltt_viable(const int number_of_lineages);

  /// Add a number of viable plus inviable lineages for the LTT plot
  void add_ltt_inviable(const int number_of_lineages);

  /// Get the LTT values of viable lineages
  const std::vector<int>& get_ltt_viables() const noexcept { return m_ltt_viables; }

  /// Get the LTT values of inviable lineages
  const std::vector<int>& get_ltt_inviables() const noexcept { return m_ltt_inviables; }

  std::vector<std::pair<double, double>> convert_ltt_to_nltt(vector<int> lineages_through_time);

private:
  /// The lineages through time
  std::vector<int> m_ltt_viables;
  std::vector<int> m_ltt_inviables;
};

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace jobo

#endif // JOBO_RESULTS_H
