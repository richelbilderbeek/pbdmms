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

jobo::results::results()
  : m_ltt{}
{
}

void jobo::results::add_ltt(const int number_of_lineages)
{
  if (number_of_lineages < 0)
  {
    throw std::invalid_argument("number of lineages cannot be negative");
  }
  m_ltt.push_back(number_of_lineages);
}


std::ostream& jobo::operator<<(std::ostream& os, const results& r) noexcept
{
  os << r.get_ltt();
  return os;
}
