#ifndef SADO_INDIVIDUAL_VERTEX_H
#define SADO_INDIVIDUAL_VERTEX_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_individual.h"
#include "sado_indiv_graph.h"

namespace sado {
vert_desc add_individual_vertex(const sado::indiv& v, indiv_graph& g) noexcept;

std::vector<sado::indiv> get_individual_vertexes(const indiv_graph& g) noexcept;
} //~namespace sado

#endif // SADO_INDIVIDUAL_VERTEX_H
