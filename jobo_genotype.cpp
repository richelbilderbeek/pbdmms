#include "jobo_genotype.h"

double jobo::calc_fitness(const genotype& g)
{
  assert(is_valid(g));
  for (int i=0; i!=g_size; i+=2)
  {
    const char a{g[i+0]};
    const char b{g[i+1]};
    if (std::islower(a) && std::isupper(b)) return 0.0;
  }
  return 1.0;
}

bool is_lowercase_character(const char c) noexcept
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


bool is_uppercase_character(const char c) noexcept
{
  return c >= 'A' && c <= 'Z';
}
