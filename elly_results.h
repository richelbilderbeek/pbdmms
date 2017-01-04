#ifndef ELLY_RESULTS_H
#define ELLY_RESULTS_H

#include <iosfwd>
#include <vector>
#include "elly_result.h"
#include "elly_fwd.h"

namespace elly {

class results
{
public:
  results(const std::vector<result>& r);

private:

  std::vector<result> m_results;

  friend std::ostream& operator<<(std::ostream& os, const results& r) noexcept;
};

results get_results(const simulation& s);
results get_results(const populations& s);

///Save results with mainland extinction to file suitable to be read by DAISIE
void save_to_daisie_with_main_ext(
  const results& r,
  const std::string& filename = "daisy_with_main_ext.csv"
);

///Save results without mainland extinction to file suitable to be read by DAISIE
void save_to_daisie_without_main_ext(
  const results& r,
  const std::string& filename = "daisy_without_main_ext.csv"
);

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace elly


#endif // ELLY_RESULTS_H
