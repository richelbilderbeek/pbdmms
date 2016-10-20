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
) : m_genotype{genotype},m_fitness{0},m_mutation_rate{0.2}
{
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
    //create number of loci to use in function
    int n = sz;
    //use get_random_int function to get as many random numbers as loci with 1 seed
    std::vector<int> n_loci_ints = (get_random_ints(rng_engine, n));
    //check if number is even or odd
    if (n_loci_ints[i] % 2 != 0)
     {
     kid[i] = {p[i]};
     }
    else kid[i] = {q[i]};
    }
  return kid;
}

genotype jobo::mutation_at_one_locus(
  const genotype& r,
  const double mutation_rate,
  std::mt19937& rng_engine
  )
  {
  //Make loop to include all loci
  const int sz{static_cast<int>(r.size())};
  genotype v = r;
  for (int i=0; i!=sz; i+=1)
    {
    //use get_random_doubles function to get as many random numbers between 0 and 1 as loci with 1 seed
    std::vector<double> n_loci_doubles = (get_random_doubles(rng_engine, sz));
    //check if random double is lower or higher than mutation_rate

    //TODO try to allow only one mutation to happen in this function
    //while(v == r)
    //{
      if (n_loci_doubles[i] <= mutation_rate)
        {
        //if locus was lowercase letter
        if('a'<=r[i] && r[i]<='z')
        //if(r[i]>=’a’ && r[i]<=’z’)
        //if (islower(r[i]))
          {
          v[i]=char(((int)r[i])-32);
          //r[i] = toupper(r[i]);
          }
        //if locus was uppercase letter
        else v[i]=char(((int)r[i])+32);
        }
    //}
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
const individual aftermutation(mutation_at_one_locus(r,mutation_rate,rng_engine));
return aftermutation;
}






/*  // TODO
  // make a mutation rate parameter for each locus (for now all the same)
  // (dependent on number of loci!)
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
