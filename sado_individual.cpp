#include "sado_individual.h"

#include <iostream>

std::list<sado::indiv> sado::pop; //GLOBAL

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
