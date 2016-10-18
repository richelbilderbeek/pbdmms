#ifndef JOBO_INDIVIDUAL_H
#define JOBO_INDIVIDUAL_H

#include <string>

using genotype = std::string;

namespace jobo {

class individual
{
public:
  individual(
    const std::string& genotype = ""
  );

  int get_n_loci() const noexcept { return static_cast<int>(m_genotype.size()); }
  const std::string& get_genotype() const noexcept {return m_genotype;}
  int calc_fitness(){return m_fitness;}

private:
  genotype m_genotype;
  int m_fitness;
};

///Creates a kid from random recombination of parents' genotype
///Will throw if genotypes are of different lengths
individual create_offspring(const individual& mother, const individual& father);

///Randomly combines genotypes mother and father
///Will throw if genotypes are of different lengths
genotype recombine(const genotype& p, const genotype& q);

// creates mutation in one of the loci in a certain genotype
// genotype mutation_one_locus(const genotype&p);

int calc_fitness(genotype s);

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_INDIVIDUAL_H
