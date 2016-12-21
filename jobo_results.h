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

  /// Add a number of lineages for the LTT plot
  void add_ltt(const int number_of_lineages);

  /// Get the LTT values
  const std::vector<int>& get_ltt() const noexcept { return m_ltt; }

private:
  /// The lineages through time
  std::vector<int> m_ltt;
};

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace jobo

#endif // JOBO_RESULTS_H
