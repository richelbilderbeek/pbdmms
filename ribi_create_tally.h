#ifndef RIBI_CREATE_TALLY_H
#define RIBI_CREATE_TALLY_H

#include <map>
#include <vector>

namespace ribi {

///Create a tally of value occurrances
///For example {"A", "A", "B"} would result in {{"A",2},{"B",1}}
///The sum of the tally frequencies will be equal to the length of the input vector
template <class T>
std::map<T, int> create_tally(
  const std::vector<T>& v
) noexcept
{
  std::map<T, int> m;
  for (const auto& i: v)
  {
    const auto iter = m.find(i);
    if (iter == std::end(m))
    {
      m.insert(std::make_pair(i, 1));
    }
    else { ++m[i]; }
  }
  return m;
}

} //~namespace ribi

#endif // RIBI_CREATE_TALLY_H
