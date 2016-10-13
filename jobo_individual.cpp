#include "jobo_individual.h"
#include "jobo_parameters.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>

jobo::individual::individual(const std::string& genotype
) : m_genotype{genotype},m_fitness{0}
{
}

int jobo::calc_fitness(std::string genotype)
{
  if (genotype.size() % 2 == 1)
  {
    throw std::invalid_argument("genotype length must be even");
  }

  //check for each 2 characters of genotype if both letters are uppercase, then fitness = 0
  const int sz{static_cast<int>(genotype.length())};
  for (int i=0; i!=sz; i+=2)
  {
    const char a{genotype[i+0]};
    const char b{genotype[i+1]};
    if (std::isupper(a) && std::isupper(b)) return 0;
  }
  //at least one lower case letter, so fitness = 1
  return 1;
 }

/* std::string& create__mutated_offspring(std::string genotype)
{
  if (mutation_rate_locus_aA == 1)
    {
  //mutate locus a to locus A in genotype abcdef
  genotype.replace(genotype.begin(), genotype.end(), 'a', 'A');
  return genotype;
    }
}

*/
  /*mutate locus b to locus B in genotype abcdef
  genotype.replace(genotype.begin(), genotype.end(), 'b', 'B');

    mutate locus c to locus C in genotype abcdef
  genotype.replace(genotype.begin(), genotype.end(), 'c', 'C');
  */

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype();
}

bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}
