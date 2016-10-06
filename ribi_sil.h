#ifndef DISTANCER_SIL_H
#define DISTANCER_SIL_H

#include <boost/dynamic_bitset.hpp>

using sil = boost::dynamic_bitset<>;

///Count the number of possible species
int count_possible_species(std::vector<sil> p, const int max_genetic_distance) noexcept;

///Counts the number of loci that are different
int get_genetic_distance(
  const sil& a,
  const sil& b
) noexcept;

///Sort and remove duplicates
void sort_and_remove_duplicates(std::vector<sil>& p) noexcept;

#endif // DISTANCER_SIL_H
