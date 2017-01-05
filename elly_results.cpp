#include "elly_results.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include "elly_results.h"
#include "elly_result.h"
#include "elly_simulation.h"
#include "elly_species.h"
#include <fstream>
#include <cassert>

elly::results::results(const std::vector<result>& r)
  : m_results{r}
{

}

elly::results elly::get_results(const simulation& s)
{
  return get_results(s.get_populations());
}

elly::results elly::get_results(const populations& p)
{
  std::vector<species> s{p.get_species()};
  std::sort(std::begin(s), std::end(s));

  std::vector<result> r;
  r.reserve(s.size());
  std::transform(
    std::begin(s),
    std::end(s),
    std::back_inserter(r),
    [](const species& i) { return result(i); }
  );
  return r;
}

/*
///ELLY_TODO
void save_daisie_results_with_main_ext(const elly::results &r)
{
  std::ofstream ofs(filename);
  //todo define status per clade
  for(const elly::species& p: r)
    {
      if(p.get_t_colonization() != -1.0)
        {

        }
    }
}

void save_daisie_results_without_main_ext();

*/

/*
daic::input_row elly::collect_info_clade(const std::vector<species>& s)
{
  assert(all_have_same_clade_id(s));
  const daic::species_status status = conclude_status(s);
  const std::string clade_name{std::to_string(s.back().get_clade_id().get_id())};
  const int n_missing_species{0};
  const std::vector<double> branching_times = collect_branching_times(s);
  return daic::input_row(
    clade_name,
    status,
    n_missing_species,
    branching_times
  );

}

daic::input convert_to_daisie_input_with_main_ext(const elly::results& r)
{
  //Collect all clade ID's

  //Per clade ID, collect all species



  daic::input in;
  for(elly::result i: r)
  {
    elly::species species = i.m_s;
    daic::input_row ir;
    ir.m_status;
  }
}
*/
///ELLY_TODO
daic::input convert_to_daisie_input_without_main_ext(const elly::results& r);


std::ostream& elly::operator<<(std::ostream& os, const results& r) noexcept
{
  os << get_result_header() << '\n';
  std::copy(
    std::begin(r.m_results),
    std::end(r.m_results),
    std::ostream_iterator<result>(os, "\n")
  );
  return os;
}
