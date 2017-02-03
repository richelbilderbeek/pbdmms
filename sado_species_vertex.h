#ifndef SADO_SPECIES_VERTEX_H
#define SADO_SPECIES_VERTEX_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include "sado_species_graph.h"

namespace sado {
sp_vert_desc add_species_vertex(const sado::sado_species& v, species_graph& g) noexcept;

std::vector<sado_species> get_species_vertexes(const species_graph& g) noexcept;
} //~namespace sado


#endif // SADO_SPECIES_VERTEX_H
