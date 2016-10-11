#include "jobo_individual.h"
#include <string>
#include <iostream>
#include <stdexcept>

jobo::individual::individual(
  const std::string& genotype
) : m_genotype{genotype}
{
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
