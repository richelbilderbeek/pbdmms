#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>
#include <random>

jobo::individual::individual(const std::string& genotype
) : m_genotype{genotype},m_fitness{0}
{
}

int get_random_int(std::mt19937& rng_engine)
{
  std::uniform_int_distribution<int> distribution(0,100);
  const auto n = distribution(rng_engine);
  return n;
}

int jobo::calc_fitness(std::string genotype)
{
  if (genotype.size() % 2 != 0)
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
  //at least one lower case letter in the two genotype cahracters (aB,Ab or ab so fitness = 1)
  return 1;
}

jobo::individual jobo::create_offspring(
const jobo::individual& mother,const jobo::individual& father
)
{
  const genotype(p)= mother.get_genotype();
  const genotype(q)= father.get_genotype();
  //Will throw if genotypes are of different lengths
  if (p.length() != q.length())
  {
   throw std::invalid_argument("genotype length must be even");
  }
  // create individual kid
  const individual offspring (recombine (p,q));
  return offspring;
}

genotype jobo::recombine(const genotype& p, const genotype& q)
{
  //Will throw if genotypes are of different lengths
  if (p.length() != q.length())
  {
   throw std::invalid_argument("genotype length must be even");
  }
  // create genotype for kid with genotype p
  genotype kid;
  kid = p;
  assert(kid == p);
  //Make loop to include all loci
  const int sz{static_cast<int>(p.size())};

  for (int i=0; i!=sz; i+=1)
    {
    //create random int
    const unsigned int seed{42};
    std::mt19937 rng_engine(seed+i);
    int n = get_random_int(rng_engine);
    //check if number is even or odd
    if (n % 2 != 0)
     {
     kid[i] = {p[i]};
     }
    else kid[i] = {q[i]};
    }
  return kid;
}

/*
genotype jobo::mutation_one_locus(const genotype& p)
{
//Make loop to include all loci
const int sz{static_cast<int>(p.size())};
  for (int i=0; i!=sz; i+=1)
  {
  // TODO
  // make a mutation rate parameter for each locus (for now all the same)(dependent on number of loci!)
  // try to call seperate mutation rate for each different locus
  // look for each locus if mutation will occur
  // if mutation occurs, replace locus with letter of opposite size
  // after the mutation of one locus, end loop and return genotype with one mutated locus
  }
}
*/

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype();
}

bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}
