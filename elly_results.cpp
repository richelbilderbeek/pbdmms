#include "elly_results.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include "elly_result.h"
#include "elly_simulation.h"

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
  const std::vector<species> s{p.get_species()};
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
