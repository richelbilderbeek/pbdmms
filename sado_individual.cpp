#include "sado_individual.h"

#include <iostream>

void sado::indiv::print()
{
  int i;
  std::cout<<x<<" "<<p<<" "<<q<<std::endl;
  for(i=0;i<Nx;i++) std::cout<<X[i]<<" ";
  std::cout<<std::endl;
  for(i=0;i<Np;i++) std::cout<<P[i]<<" ";
  std::cout<<std::endl;
  for(i=0;i<Nq;i++) std::cout<<Q[i]<<" ";
  std::cout<<std::endl;
}

bool sado::operator==(const indiv& lhs, const indiv& rhs) noexcept
{
  return lhs._x() == rhs._x()
    && lhs._p() == rhs._p()
    && lhs._q() == rhs._q()
  ;
}
