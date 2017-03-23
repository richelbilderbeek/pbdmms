#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

#include <vector>

#include "sado_individual.h"

namespace sado
{

class population
{
public:
  population(const std::vector<individual>& initial_population = {});
  //population(const population&) = delete;
  //population& operator=(const population&) = delete;
  const auto& get_population() const noexcept { return  m_population; }
  int size() const noexcept { return m_population.size(); }
  bool empty() const noexcept { return m_population.empty();}

  ///Add an individual
  void add_indiv(const individual& i);

  void pop_back() { m_population.pop_back(); }
  const individual& operator[](const int i) const;
  individual& operator[](const int i);
  void erase(int index) {m_population.erase(std::begin(m_population) + index);}

  ///Resize the population to a lower size, get rid of the excess individuals
  ///Will throw if the size indicated is bigger than the current population size
  void downsize(const int smaller_size);
  individual& back() {return m_population.back();}

private:
  std::vector<individual> m_population;
};

double calc_rhoxp(const population& p);
double calc_rhoxq(const population& p);
double calc_rhopq(const population& p);


double calc_sp(const population& p);
double calc_sq(const population& p);
double calc_sx(const population& p);

///Create an empty population
population create_test_population_0() noexcept;

///Create a population of one individual
population create_test_population_1() noexcept;

///Create a population of two individuals of the same species
population create_test_population_2() noexcept;

///Create a population of two individuals of different species
population create_test_population_3() noexcept;

double get_mean_x(const population& p);
double get_mean_p(const population& p);
double get_mean_q(const population& p);

bool operator==(const population& lhs, const population& rhs) noexcept;
bool operator!=(const population& lhs, const population& rhs) noexcept;

} // namespace sado

#endif // SADO_POPULATION_H
