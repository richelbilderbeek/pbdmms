#ifndef SADO_SPECIES_H
#define SADO_SPECIES_H

#include "sado_individual.h"
#include "sado_id.h"
#include <iosfwd>
#include <vector>

namespace sado {

class species
{
public:
  explicit species(
      const int gen = -1,
      const std::vector<indiv>& indivs = {});

  std::vector<indiv> get_indivs() const noexcept {return m_indivs;}
  int get_generation() const noexcept {return m_generation;}
  id get_id() const noexcept {return m_id;}

  ///Is there an individual with this (individual) ID present?
  bool has_individual(const id any_id) const noexcept;

  ///Do not use this anymore. This ensures that the indivs are always sorted
  [[deprecated]]
  void add_indiv(const indiv& i) {m_indivs.push_back(i);}

  void set_generations_number(const int num) {m_generation = num;}
  bool empty() const noexcept;
  size_t size() const noexcept;
  const indiv& operator[](const int i) const;

private:
  id m_id;
  int m_generation;
  std::vector<indiv> m_indivs;

  friend void transfer_individuals(species& from, species& to);

};

///Find if the 'ancestors' contain at least one father or mother
///of one of the individuals in 'kids'
bool has_ancestor_and_kid(const species& ancestors, const species& kids);

///Transfers the individuals from 'from' to 'to'
///Assumes the species are from the same generation
void transfer_individuals(species& from, species& to);

bool operator==(const species &lhs, const species &rhs) noexcept;
bool operator!=(const species &lhs, const species &rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const species& spp) noexcept;

}//~namespace sado
#endif // SADO_SPECIES_H
