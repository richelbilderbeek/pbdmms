#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_results.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>
#include <random>
#include <vector>

using namespace std;
using namespace jobo;

jobo::results::results(
 // std::vector<int> m_ltt
)
{
}

std::ostream& jobo::operator<<(std::ostream& os, const simulation& s) noexcept
{
  os << s.get_results;
  return os;
}
