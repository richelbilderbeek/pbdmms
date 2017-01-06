#ifndef ELLY_CLADES_H
#define ELLY_CLADES_H

#include <vector>
#include "elly_clade.h"

namespace elly {

using clades = std::vector<clade>;

///Only keep those members that
/// * have colonized the island
/// * are born on the island
clades get_islanders(const clades& s);

///Instead of knowing the colonization times, we
///overestimate the colonization time to the common
///ancestor on the mainland
clades overestimate_colonization_times(const clades& c);

} //~namespace elly

#endif // ELLY_CLADES_H
