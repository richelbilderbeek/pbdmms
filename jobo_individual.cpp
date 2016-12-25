#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>
#include <random>
#include <vector>

jobo::individual::individual(const std::string& genotype
) : m_genotype{genotype},m_fitness{0}
{
}

double jobo::calc_fitness(const individual& i)
{
  return calc_fitness(i.get_genotype());
}

std::string jobo::create_initial_genotype(const int n_loci)
{
  if (n_loci < 0)
  {
    throw std::invalid_argument("n_loci must be zero or positive");
  }
  std::string genotype;
  for (int i=0; i!=n_loci; ++i)
  {
    const int char_index = i % 26;
    const char c = 'a' + char_index;
    genotype += c;
  }
  return genotype;
}

jobo::individual jobo::create_offspring(
  const jobo::individual& mother,
  const jobo::individual& father,
  std::mt19937& rng_engine
)
{
  const genotype p{mother.get_genotype()};
  const genotype q{father.get_genotype()};

  // Test if genotypes are even
  if (p.length() != q.length())
  {
    throw std::invalid_argument("genotype length must have the same length");
  }

  // Create individual kid
  const individual offspring(recombine(p,q,rng_engine));
  if (offspring.get_genotype().size() % 2 != 0)
    {
      throw std::invalid_argument("genotype length must be even");
    }
  return offspring;
}

jobo::genotype jobo::recombine(
  const genotype& p,
  const genotype& q,
  std::mt19937& rng_engine
)
{
  // Test if genotypes are even
  if (p.length() != q.length())
  {
    throw std::invalid_argument("genotype length must have the same length");
  }

  // Create genotype for kid with genotype p
  genotype kid;
  kid = p;
  assert(kid == p);

  // Make loop to include all loci
  const int sz{static_cast<int>(p.size())};
  for (int i=0; i!=sz; i+=1)
  {
    // Use get_random_int function to get as many random numbers as loci
    std::vector<int> n_loci_ints = (get_random_ints(rng_engine, sz));

    // Check if number is even or odd
    if (n_loci_ints[i] % 2 == 0)
    {
      kid[i] = {q[i]};
    }
    else kid[i] = {p[i]};
  }
  return kid;
}

jobo::genotype jobo::mutation_check_all_loci(
  const genotype& r,
  const double mutation_rate,
  std::mt19937& rng_engine
)
{
  // Make loop to include all loci
  const int sz{static_cast<int>(r.size())};
  genotype v = r;
  for (int i=0; i!=sz; i+=1)
  {
    // Use get_random_doubles to get as many random numbers as loci between 0 and 1
    std::vector<double> n_loci_doubles = (get_random_doubles(rng_engine, sz));
    // Check if random double is lower or higher than mutation_rate
    if (n_loci_doubles[i] <= mutation_rate)
    {
      // If locus is lowercase letter
      if('a'<=r[i] && r[i]<='z')
        {
        v[i]=char(((int)r[i])-32);
        }
      // If locus is uppercase letter
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
  // Create genotype r as genotype before mutation
  const genotype r{before_mutation.get_genotype()};
  const individual aftermutation(mutation_check_all_loci(r,mutation_rate,rng_engine));
  return aftermutation;
}

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype();
}

bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& jobo::operator<<(std::ostream& os, const individual& i) noexcept
{
  os << i.get_genotype();
  return os;
}

