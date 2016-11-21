#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iosfwd>
#include "ribi_pin.h"
#include "ribi_sil.h"
#include "ribi_probability.h"

namespace ribi {

struct individual
{
  /// Species Identity Loci
  using sil_t = sil;
  /// Phylogeny Inference Nucleotides
  using pin_t = pin;

  individual(
    const pin_t& any_pin,
    const sil_t& any_sil
  );

  const pin_t& get_pin() const noexcept { return m_pin; }
        pin_t& get_pin()       noexcept { return m_pin; }

  const sil_t& get_sil() const noexcept { return m_sil; }
        sil_t& get_sil()       noexcept { return m_sil; }

  private:
  ///Phylogeny Inference Nucleotides: the neutral DNA
  ///use to infer phylogenies
  pin_t m_pin;

  ///Species Identity Loci: determines if an individual
  ///can produce viable offspring with others
  sil_t m_sil;

};

///Can the two individuals mate?
bool can_mate(
  const individual& a,
  const individual& b,
  const int max_genetic_distance
);

///Count the number of ring species,
///where two individuals must have at most 'max_genetic_distance'
///genetic difference to be called the same species
std::vector<int> count_abundances(
  std::vector<individual> p,
  const int max_genetic_distance
) noexcept;

///Count the possible number of ring species, where two individuals must
///have at most 'max_genetic_distance' genetic difference to be called the same species
int count_possible_species(std::vector<individual> p, const int max_genetic_distance) noexcept;

///Count the number of ring species, where two individuals must have at most 'max_genetic_distance'
///genetic difference to be called the same species
int count_species(std::vector<individual> p, const int max_genetic_distance) noexcept;

///P:   0000
///Q:   1111
///     0101
///kid: 1010
individual create_offspring(
  const individual& p,
  const individual& q,
  const boost::dynamic_bitset<>& inherit_pin_from_p,
  const boost::dynamic_bitset<>& inherit_sil_from_p
);

///Counts the number of loci that are different
int get_genetic_distance(
  const individual& a,
  const individual& b
) noexcept;

///Mutate an individual
/// @param plpmp Per Locus PIN Mutation Probability
/// @param plsmp Per Locus SIL Mutation Probability
void mutate(
  individual& i,
  const probability plpmp,
  const probability plsmp,
  std::mt19937& rng_engine
);

///Mutate an individual its PINs only
/// @param plpmp Per Locus PIN Mutation Probability
void mutate_pins(
  individual& i,
  const probability plpmp,
  std::mt19937& rng_engine
);

///Mutate an individual its SILs only
/// @param plsmp Per Locus SIL Mutation Probability
void mutate_sils(
  individual& i,
  const probability plsmp,
  std::mt19937& rng_engine
);

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const individual& i) noexcept;

} //~namespace ribi

#endif // INDIVIDUAL_H
