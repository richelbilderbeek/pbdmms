#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cctype>
#include <string>
#include <random>

using namespace std;
using namespace jobo;

jobo::parameters::parameters(
    const int population_size,
    const int seed,
    const double mutation_rate,
    const int duration,
    const int loci
    )
  : m_population_size{population_size},
    m_seed{seed},
    m_mutation_rate{mutation_rate},
    m_duration{duration},
    m_loci{loci}
{
  // Give exceptions for impossible parameter settings
  if (population_size < 0)
  {
    throw std::invalid_argument("population_size must be positive");
  }
  if (mutation_rate < 0)
  {
    throw std::invalid_argument("mutation_rate must be positive");
  }
  if (mutation_rate > 1)
  {
    throw std::invalid_argument("mutation_rate can't be higher than 1");
  }
  if (duration < 0)
  {
    throw std::invalid_argument("duration must be positive");
  }
}

bool jobo::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return lhs.get_population_size() == rhs.get_population_size();
}

bool jobo::operator!=(const parameters& lhs, const parameters& rhs) noexcept
{
  return !(lhs == rhs);
}

