#ifndef ELLY_HELPER_H
#define ELLY_HELPER_H

#include <algorithm>
#include <vector>

namespace elly {

template <class T>
std::vector<T> get_sorted(std::vector<T> v)
{
  std::sort(std::begin(v), std::end(v));
  return v;
}

template <class T>
std::vector<T> get_with_duplicates_removed(std::vector<T> v)
{
  const auto new_end = std::unique(std::begin(v), std::end(v));
  v.erase(new_end, std::end(v));
  return v;
}

template <class T>
std::vector<T> get_with_zeroes_removed(std::vector<T> v)
{
  const auto new_end = std::remove(std::begin(v), std::end(v), 0.0);
  v.erase(new_end, std::end(v));
  return v;
}



} //~namespace elly


#endif // ELLY_HELPER_H
