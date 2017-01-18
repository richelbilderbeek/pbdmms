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
  : m_ltt_viables{},
    m_ltt_inviables{}

{
}

void jobo::results::add_ltt_viable(const int number_of_lineages)
{
  if (number_of_lineages < 0)
  {
    throw std::invalid_argument("number of lineages cannot be negative");
  }
  m_ltt_viables.push_back(number_of_lineages);
}

void jobo::results::add_ltt_inviable(const int number_of_lineages)
{
  if (number_of_lineages < 0)
  {
    throw std::invalid_argument("number of lineages cannot be negative");
  }
  m_ltt_inviables.push_back(number_of_lineages);
}

std::ostream& jobo::operator<<(std::ostream& os, const results& r) noexcept
{
  for (const int i: r.get_ltt_viables())
  {
    os << i << ' ';
  }
  os << '\n';
  return os;
}
