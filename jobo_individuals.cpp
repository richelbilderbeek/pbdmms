#include "jobo_individuals.h"
#include <iostream>
#include <stdexcept>
#include <vector>

std::vector<double> calc_fitnesses(const individuals& population) noexcept
{
  std::vector<double> fitnesses;
  fitnesses.reserve(population.size());
  HIERO
}

std::ostream& jobo::operator<<(std::ostream& os, const individuals& individuals) noexcept
{
  for (const auto& individual: individuals)
  {
    os << individual << '\n';
  }
  return os;
}
