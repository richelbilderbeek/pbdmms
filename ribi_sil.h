#ifndef DISTANCER_SIL_H
#define DISTANCER_SIL_H

#include <boost/dynamic_bitset.hpp>

namespace ribi {

using sil = boost::dynamic_bitset<>;

///Can the two SILs mate?
bool can_mate(
  const sil& a,
  const sil& b,
  const int max_genetic_distance
);

///Count the number of possible species
int count_possible_species(
  std::vector<sil> p,
  const int max_genetic_distance
) noexcept;

///Create a SIL from a std::string
sil create_sil(const std::string& s);

///Counts the number of loci that are different
int get_genetic_distance(
  const sil& a,
  const sil& b
) noexcept;

///Sort and remove duplicates
void sort_and_remove_duplicates(std::vector<sil>& p) noexcept;

} //~namespace ribi

#endif // DISTANCER_SIL_H
