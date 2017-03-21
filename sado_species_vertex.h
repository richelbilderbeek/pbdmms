#ifndef SADO_SPECIES_VERTEX_H
#define SADO_SPECIES_VERTEX_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include "sado_ancestry_graph.h"


namespace sado {

//[[deprecated]]
std::vector<species> get_species_vertexes(const ancestry_graph& g) noexcept;

} //~namespace sado


#endif // SADO_SPECIES_VERTEX_H
