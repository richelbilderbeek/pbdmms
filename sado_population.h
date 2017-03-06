#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

#include <vector>

#include "sado_individual.h"
#include "sado_pedigree.h"

namespace sado
{

class population
{
public:
  population(const std::vector<indiv>& initial_population = {});
  const auto& get_pedigree() const noexcept { return  m_pedigree; }
  const auto& get_population() const noexcept { return  m_population; }
  int size() const noexcept { return m_population.size(); }
  bool empty() const noexcept { return m_population.empty();}
  void add_indiv(const indiv& i);
  void pop_back() { m_population.pop_back(); }
  const indiv& operator[](const int i) const;
  indiv& operator[](const int i);
  void erase(int index) {m_population.erase(std::begin(m_population) + index);}

  ///Resize the population to a lower size, get rid of the excess individuals
  ///Will throw if the size indicated is bigger than the current population size
  void downsize(const int smaller_size);
  indiv& back() {return m_population.back();}

private:
  pedigree m_pedigree;
  std::vector<indiv> m_population;
};


using offspring = std::vector<indiv>;
using fathers = std::vector<indiv>;

double get_mean_x(const population &p);
double get_mean_p(const population &p);
double get_mean_q(const population &p);

bool operator==(const population& lhs, const population& rhs) noexcept;
bool operator!=(const population& lhs, const population& rhs) noexcept;

} // namespace sado

#endif // SADO_POPULATION_H
