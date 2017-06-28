#ifndef PHYG_HELPER_H
#define PHYG_HELPER_H

#include <set>
#include <vector>

namespace phyg {

///Convert std::vector to std::set
template <class T>
std::set<T> to_set(const std::vector<T>& v)
{
  return std::set<T>(std::begin(v), std::end(v));
}

} //~namespace phyg


#endif // PHYG_HELPER_H
