#ifndef ELLY_RESULTS_H
#define ELLY_RESULTS_H

#include <iosfwd>
#include <vector>
#include "elly_result.h"
#include "elly_fwd.h"
#include "elly_species.h"
#include "daic_input.h"
#include "daic_input_row.h"

namespace elly {

class results
{
public:
  results(const std::vector<result>& r = {});

private:

  std::vector<result> m_results;

  friend std::ostream& operator<<(std::ostream& os, const results& r) noexcept;
};

results get_results(const simulation& s);
results get_results(const populations& s);

species find_youngest_parent(const vector<species> s);
std::vector<species> collect_kids(species parent, std::vector<species> s);

std::vector<double> collect_branching_times(std::vector<species> s);

daic::input_row collect_info_clade(const std::vector<species>& s);

///ELLY_TODO
daic::input convert_to_daisie_input_with_main_ext(const results& r);
///ELLY_TODO
daic::input convert_to_daisie_input_without_main_ext(const results& r);

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace elly


#endif // ELLY_RESULTS_H
