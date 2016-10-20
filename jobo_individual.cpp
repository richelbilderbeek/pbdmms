#include "jobo_individual.h"
#include "jobo_parameters.h"
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
  const jobo::individual& mother,
  const jobo::individual& father,
  std::mt19937& rng_engine
)
{
  const genotype p{mother.get_genotype()};
  const genotype q{father.get_genotype()};
  //Will throw if genotypes are of different lengths
  if (p.length() != q.length())
  {
   throw std::invalid_argument("genotype length must be even");
  }
  // create individual kid
  const individual offspring(recombine(p,q,rng_engine));
  return offspring;
}

genotype jobo::recombine(
  const genotype& p,
  const genotype& q,
  std::mt19937& rng_engine
)
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
    //const unsigned int seed{42};
    //std::mt19937 rng_engine(seed+i);
    int n = get_random_int(rng_engine);
    //check if number is even or odd
    if (n % 2 == 1)
     {
     kid[i] = {p[i]};
     }
    else kid[i] = {q[i]};
    }
  return kid;
}

/*
//MUTATION THEORY

// create_mutated_offspring needs to (create difference in)/replace-
// one locus with the locus of opposite size
// in the offspring genotype string (the copy/outcome of the parent(s) genotype strings).
// The parameter mutation rate (for each locus, but with same rate?, dependent on number of loci)
// will be used to determine if mutation will happen.

//mutaton rate per locus seems necessary but not really usefull...? example mutation_rate_locus_aA

 //implement following mutation step in create_offspring
std::string& create__mutated_offspring(std::string genotype)
{
  if (mutation_rate_locus_aA == 1)
    {
    // mutate locus a to locus A in genotype abcdef
    genotype.replace(genotype.begin(), genotype.end(), 'a', 'A');
    }
  if (mutation_rate_locus_bB == 1)
    {
    // mutate locus b to locus B in genotype abcdef
    genotype.replace(genotype.begin(), genotype.end(), 'b', 'B');
    }
  if (mutation_rate_locus_cC == 1)
    {
    // mutate locus c to locus C in genotype abcdef
    genotype.replace(genotype.begin(), genotype.end(), 'c', 'C');
    }
  return genotype;
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
