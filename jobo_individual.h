#ifndef JOBO_INDIVIDUAL_H
#define JOBO_INDIVIDUAL_H

#include <string>

namespace jobo {

class individual
{
public:
  individual(
    const std::string& genotype = ""
  );

  int get_n_loci() const noexcept { return static_cast<int>(m_genotype.size()); }
  const std::string& get_genotype() const noexcept { return m_genotype;}
  int calc_fitness(){ return m_fitness;}

private:
  std::string m_genotype;
  int m_fitness;
};

int calc_fitness(std::string s);

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_INDIVIDUAL_H
