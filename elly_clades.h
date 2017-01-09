#ifndef ELLY_CLADES_H
#define ELLY_CLADES_H

#include <vector>
#include "elly_clade.h"

namespace elly {

using clades = std::vector<clade>;

///Count the number of clades that have zero species
int count_empty(const clades& cs) noexcept;

///Only keep those members that
/// * have colonized the island
/// * are born on the island
clades get_islanders(const clades& s);

///Instead of knowing the colonization times, we
///overestimate the colonization time to the common
///ancestor on the mainland
clades to_reality(const clades& c);

clades get_non_empty_clades(const clades& cs) noexcept;

} //~namespace elly

#endif // ELLY_CLADES_H
