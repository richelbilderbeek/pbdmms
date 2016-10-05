#ifndef POPULATION_H
#define POPULATION_H

#include <algorithm>
#include <map>
#include <vector>
#include "distancer_individual.h"
#include "distancer_sil.h"

using population = std::vector<individual>;

std::map<sil,int> tally_sils(const population& any_population) noexcept;

template <class T>
int sum_tally(const std::map<T,int>& m) noexcept
{
  return static_cast<int>(
    std::accumulate(
      std::begin(m), std::end(m), 0,
      [](int sum, const std::pair<T,int>& p)
      {
        return sum + p.second;
      }
    )
  );
}

#endif // POPULATION_H
