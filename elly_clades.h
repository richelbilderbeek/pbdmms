#ifndef ELLY_CLADES_H
#define ELLY_CLADES_H

#include <vector>
#include "elly_clade.h"

namespace elly {

using clades = std::vector<clade>;

clades get_island_borne(const clades& s);

} //~namespace elly

#endif // ELLY_CLADES_H
