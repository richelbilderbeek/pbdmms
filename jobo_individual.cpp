#include "jobo_individual.h"
#include "jobo_parameters.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>

jobo::individual::individual(
   const std::string& genotype
) : m_genotype{genotype},m_fitness{0}
{
   // if (genotype.size() % 2 != 0)
   // {
   //   throw std::invalid_argument("genotype must be an even number");
   // }
}

int jobo::calc_fitness(std::string genotype)
{
  if (genotype.size() % 2 == 1)
  {
    throw std::invalid_argument("genotype length must be even");
  }

  const int sz{static_cast<int>(genotype.length())};
  for (int i=0; i!=sz; i+=2)
  {
    const char a{genotype[i+0]};
    const char b{genotype[i+1]};
    if (std::isupper(a) && std::isupper(b)) return 0;
  }
  return 1;
 }

  /*
    std::string temp;
    // look if string has 2,4 or 6 characters
    assert (genotype.size() == 4||genotype.size() == 2||genotype.size() == 6);

    // for-loop to take each time 2 characters of the genotype string
    const int sz{static_cast<int>(genotype.length())};
    for ( int i = 0; i != sz; ++i )
    {
       // Add the current character to temp to store 2 characters
       temp += genotype[ i ];
       if ( temp.length() >= 2 )
       {
         // look if both characters are uppercase letters: fitness 0
         if(std::isupper(temp[0]) && std::isupper(temp[1]))
         {
           // returns 0 if both characters are uppercase letters: fitness 0
           return 0;
         }
         // Reset temp
         temp = "";
       }
    }
    //returns 1 if one of both characters is a lowercase letter: fitness 1
    return 1;
  */

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype();
}


bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}
