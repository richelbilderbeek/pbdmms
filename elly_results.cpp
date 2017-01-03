#include "elly_results.h"

#include <iterator>
#include <iostream>
#include "elly_result.h"


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
