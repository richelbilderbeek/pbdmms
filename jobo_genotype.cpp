#include "jobo_genotype.h"

#include <cassert>
#include <stdexcept>

double jobo::calc_fitness(const genotype& g)
{
  if (!is_valid(g))
  {
    throw std::invalid_argument("Invalid genotype");
  }
  const int sz{static_cast<int>(g.size())};
  for (int i=0; i!=sz; i+=2)
  {
    assert(i + 0 < static_cast<int>(g.size()));
    assert(i + 1 < static_cast<int>(g.size()));
    const char a{g[i+0]};
    const char b{g[i+1]};
    if (std::islower(a) && std::isupper(b)) return 0.0;
  }
  return 1.0;
}

bool jobo::is_lowercase_character(const char c) noexcept
{
  return c >= 'a' && c <= 'z';
}

bool jobo::is_valid(const genotype& g) noexcept
{
  if (g.size() % 2 != 0) return false;
  for (const char c: g)
  {

    if (!is_lowercase_character(c) && !is_uppercase_character(c)) return false;
  }
  return true;
}


bool jobo::is_uppercase_character(const char c) noexcept
{
  return c >= 'A' && c <= 'Z';
}
