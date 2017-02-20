#ifndef SADO_SPECIES_H
#define SADO_SPECIES_H

#include "sado_individual.h"
#include "sado_id.h"
#include <vector>

namespace sado {

class species
{
public:
  species(
      const int gen = -1,
      const std::vector<indiv>& indivs = {});

  std::vector<indiv> get_indivs() const noexcept {return m_indivs;}
  int get_generation() const noexcept {return m_generation;}
  id get_id() const noexcept {return m_id;}

  void add_indiv(const indiv& i) {m_indivs.push_back(i);}
  void set_generations_number(const int num) {m_generation = num;}
  bool empty() const noexcept;
  size_t size() const noexcept;
  const indiv& operator[](const int i) const;


private:
  id m_id;
  int m_generation;
  std::vector<indiv> m_indivs;


};

bool operator==(const species &lhs, const species &rhs) noexcept;
bool operator!=(const species &lhs, const species &rhs) noexcept;


}//~namespace sado
#endif // SADO_SPECIES_H
