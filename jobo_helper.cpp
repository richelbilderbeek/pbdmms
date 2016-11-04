#include "jobo_helper.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>

void hello_jobo() noexcept
{
  std::cout <<"hello world" << '\n';
}

int add(int a, int b) noexcept
{
   return a + b;
}

double divide(double a, double b)
{
    if(b==0.0){
      throw std::invalid_argument("can't divide by 0");
    }
    return a/b;
}

int count_good_species(std::vector<int> chances)
{
int sz = 6;
int n_good_species = 1;
const int y{static_cast<int>(chances.size())};
for (int i=0; i!=sz-1; i+=1)
{
  if(chances[i]!=0) ++n_good_species;
}

if(n_good_species == sz)
{
  for (int i=y-1; i>(sz-2); i--)
  {
    if(chances[i]!=2) --n_good_species;
    if (n_good_species == 2)
    {
      break;
    }
  }
}
return n_good_species;
}
