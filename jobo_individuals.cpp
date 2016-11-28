#include "jobo_individuals.h"
#include <iostream>
#include <stdexcept>
#include <vector>


std::ostream& jobo::operator<<(std::ostream& os, const individuals& individuals) noexcept
{
  for (const auto& individual: individuals)
  {
    os << individual << '\n';
  }
  return os;
}

