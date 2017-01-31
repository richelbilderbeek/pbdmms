#ifndef RIBI_TALLY_TO_STR_H
#define RIBI_TALLY_TO_STR_H

#include <cassert>
#include <map>
#include <sstream>

namespace ribi {

///Converts a tally to a one-line std::string
///For example, {{"A", 2}, {"B", 3}}, becomes "A: 2, B: 3"
template <class T>
std::string tally_to_str(
  const std::map<T,int>& tally
) noexcept
{
  if (tally.empty()) return "";
  std::stringstream s;
  for (const auto p: tally)
  {
    s << p.first << ": " << p.second << ", ";
  }
  std::string t{s.str()};
  //Strip the ' ,' from the end
  assert(t.size() >= 2);
  t.resize(t.size() - 2);
  return t;
}

} //~namespace ribi

#endif // RIBI_TALLY_TO_STR_H
