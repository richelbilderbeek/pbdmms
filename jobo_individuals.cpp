#include "jobo_individuals.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include "jobo_individual.h"

std::vector<double> jobo::calc_fitnesses(const individuals& population) noexcept
{
  std::vector<double> fitnesses;
  fitnesses.reserve(population.size());
  std::transform(
    std::begin(population),
    std::end(population),
    std::back_inserter(fitnesses),
    [](const individual& i) { return calc_fitness(i); }
  );
  assert(fitnesses.size() == population.size());
  return fitnesses;
}

bool jobo::is_viable(const individuals& population)
{
  const auto fitnesses = calc_fitnesses(population);
  return std::count_if(
    std::begin(fitnesses),
    std::end(fitnesses),
    [](const double f) { return f > 0.0; }
  ) >= 2;
}

std::ostream& jobo::operator<<(std::ostream& os, const individuals& individuals) noexcept
{
  for (const auto& individual: individuals)
  {
    os << individual << '\n';
  }
  return os;
}

