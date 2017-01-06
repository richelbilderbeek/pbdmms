#ifndef ELLY_RESULTS_H
#define ELLY_RESULTS_H

#include <iosfwd>
#include <map>
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

  const auto& get() const noexcept { return m_results; }

private:

  std::vector<result> m_results;

  friend std::ostream& operator<<(std::ostream& os, const results& r) noexcept;
};

results get_results(const simulation& s);
results get_results(const populations& s);

species find_youngest_parent(const std::vector<species> s);

///Find all the species that are descendants of the parent
///or descentants of its (descendants of ...) descendants
std::vector<species> collect_kids(
  const species& parent,
  const std::vector<species>& population);

std::vector<double> collect_branching_times(const clade& s);

///Extract all the species from the results, put them in their own clades
std::map<clade_id, std::vector<species>> collect_clades_as_map(const results& r);
std::vector<clade> collect_clades_as_vector(const results& r);

///Collect all species that have migrated and diversified
std::vector<species> collect_ancestors(const std::vector<species>& s);

daic::species_status conclude_status(const std::vector<species> &s);

daic::species_status conclude_status(const clade& s);

daic::input_row collect_info_clade(const clade& s);

///ELLY_TODO
daic::input convert_to_daisie_input_with_main_ext(const results& r);
///ELLY_TODO
daic::input convert_to_daisie_input_without_main_ext(const results& r);


//void remove(
//  const species& parent,
//  std::vector<species>& population
//);

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace elly


#endif // ELLY_RESULTS_H
