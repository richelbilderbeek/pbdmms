#ifndef JOBO_INDIVIDUAL_H
#define JOBO_INDIVIDUAL_H

#include <random>
#include "jobo_genotype.h"

namespace jobo {

class individual
{
public:
  individual(
    const std::string& genotype = ""
  );

  int get_n_loci() const noexcept { return static_cast<int>(m_genotype.size()); }
  const std::string& get_genotype() const noexcept {return m_genotype;}
  double get_fitness(){return m_fitness;}

private:
  genotype m_genotype;
  double m_fitness;
};

///Create a genotype of length 'n_loci', creating a genotype like
///'abcdef...' to have the correct length
std::string create_initial_genotype(const int n_loci);

///Creates a kid from random recombination of parents' genotype
///Will throw if genotypes are of different lengths
individual create_offspring(
  const individual& mother,
  const individual& father,
  std::mt19937& rng_engine
);

///Creates individual with mutation at one loci in the genotype
individual create_mutation(
  const jobo::individual& before_mutation,
  const double mutation_rate,
  std::mt19937& rng_engine
);

///Randomly combines genotypes mother and father
///Will throw if genotypes are of different lengths
genotype recombine(
  const genotype& p,
  const genotype& q,
  std::mt19937& rng_engine
);

/// Creates mutation in one of the loci in a certain genotype
genotype mutation_check_all_loci(
  const genotype &r,
  const double mutation_rate,
  std::mt19937& rng_engine
);

int calc_fitness(const genotype& s);

std::ostream& operator<<(std::ostream& os, const individual& i) noexcept;

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_INDIVIDUAL_H
