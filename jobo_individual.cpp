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

int jobo::calc_fitness(std::string genotype)
{
  int n_low_fitness{1};
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
    if (std::isupper(a) && std::isupper(b)) --n_low_fitness;
  }
  //at least one lower case letter in the two genotype cahracters (aB,Ab or ab so fitness = 1)
  return n_low_fitness;
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
    //use get_random_int function to get as many random numbers as loci
    std::vector<int> n_loci_ints = (get_random_ints(rng_engine, sz));
    //check if number is even or odd
    if (n_loci_ints[i] % 2 == 0)
    {
      kid[i] = {q[i]};
    }
    else kid[i] = {p[i]};
  }
  return kid;
}

genotype jobo::mutation_check_all_loci(
  const genotype& r,
  const double mutation_rate,
  std::mt19937& rng_engine
  )
  {
  //make loop to include all loci
  const int sz{static_cast<int>(r.size())};
  genotype v = r;
  for (int i=0; i!=sz; i+=1)
  {
    //use get_random_doubles to get as many random numbers as loci between 0 and 1
    std::vector<double> n_loci_doubles = (get_random_doubles(rng_engine, sz));
    //check if random double is lower or higher than mutation_rate
    if (n_loci_doubles[i] <= mutation_rate)
    {
    //if locus was lowercase letter
    if('a'<=r[i] && r[i]<='z')
    {
      v[i]=char(((int)r[i])-32);
    }
    //if locus was uppercase letter
    else v[i]=char(((int)r[i])+32);
    }
  }
return v;
}

jobo::individual jobo::create_mutation(
const jobo::individual& before_mutation,
const double mutation_rate,
std::mt19937& rng_engine
)
{
const genotype r{before_mutation.get_genotype()};
const individual aftermutation(mutation_check_all_loci(r,mutation_rate,rng_engine));
return aftermutation;
}

// Maybe different mutation rate for each locus (not) necessary
// Number of mutation rates dependent on loci

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype();
}

bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}


