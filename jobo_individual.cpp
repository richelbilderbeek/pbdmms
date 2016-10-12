#include "jobo_individual.h"
#include "jobo_parameters.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>

jobo::individual::individual(
   const std::string& genotype
) : m_genotype{genotype}
{

}

int jobo::calc_fitness(std::string genotype)
{
    // look if string has 2 characters
    assert (genotype.size() == 2);

    if(std::isupper(genotype[0]) && std::isupper(genotype[1]))
    {
      // returns 0 if both characters are uppercase letters: fitness 0
      return 0;
    }
    //returns 1 if one of both characters is a lowercase letter: fitness 1
    return 1;
}

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype()
 ;
}

bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}
