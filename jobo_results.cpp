#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_results.h"
#include "pbd_nltt.h"
#include "pbd_ltt.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <cctype>
#include <random>
#include <vector>

using namespace std;
using namespace jobo;

jobo::results::results()
  : m_ltt_viables{},
    m_ltt_inviables{},
    m_nltt_viables{},
    m_nltt_inviables{}
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

void jobo::results::add_nltt_viables(const vector<int> lineages_through_t)
{
  m_nltt_viables = convert_ltt_to_nltt(lineages_through_t);
  //std::cout << convert_ltt_to_nltt(lineages_through_t) << '\n';
}

void jobo::results::add_nltt_inviables(const vector<int> lineages_through_t)
{
  m_nltt_inviables = convert_ltt_to_nltt(lineages_through_t);
}

pbd::nltt jobo::convert_ltt_to_nltt(vector<int> lineages_through_t)
{
  const int ltt_sz{static_cast<int>(lineages_through_t.size())};
  pbd::ltt old_ltt;
  for (int i=0; i!=ltt_sz; ++i)
  {
    old_ltt.add_timepoint(i, lineages_through_t[i]);
  }
  const pbd::nltt new_nltt = pbd::convert_to_nltt(old_ltt);
  //std::cout << new_nltt << '\n';
  return pbd::convert_to_nltt(old_ltt);
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
