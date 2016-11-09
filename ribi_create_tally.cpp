#include "ribi_create_tally.h"
#include <map>

std::vector<int> ribi::create_tally(const std::vector<int>& v) noexcept
{
  std::map<int, int> m;
  for (const auto i: v)
  {
    const auto iter = m.find(i);
    if (iter == std::end(m))
    {
      m.insert(std::make_pair(i, 1));
    }
    else { ++m[i]; }
  }

  std::vector<int> t;
  t.reserve(m.size());
  for (const auto p: m) {
    t.push_back(p.second);
  }
  return t;
}
