#ifndef JOBO_RESULTS_H
#define JOBO_RESULTS_H

#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <stdexcept>
#include <vector>

#include "pbd_nltt.h"




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

  /// Add all numbers of viable lineages for the nLTT plot
  void add_nltt_viables(vector<int> lineages_through_t);

  /// Add all numbers of inviable lineages for the nLTT plot
  void add_nltt_inviables (vector<int> lineages_through_t);

  /// Get the LTT values of viable lineages
  const std::vector<int>& get_ltt_viables() const noexcept { return m_ltt_viables; }

  /// Get the LTT values of inviable lineages
  const std::vector<int>& get_ltt_inviables() const noexcept { return m_ltt_inviables; }

  /// Get the nLTT values of viable lineages
  const pbd::nltt& get_nltt_viables() const noexcept {return m_nltt_viables; }

  /// Get the nLTT values of inviable lineages
  const pbd::nltt& get_nltt_inviables() const noexcept {return m_nltt_inviables; }

private:
  /// The lineages through time
  std::vector<int> m_ltt_viables;
  std::vector<int> m_ltt_inviables;
  /// The normalized lineages through time
  pbd::nltt m_nltt_viables;
  pbd::nltt m_nltt_inviables;
};

pbd::nltt convert_ltt_to_nltt(vector<int> lineages_through_t);

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace jobo

#endif // JOBO_RESULTS_H
