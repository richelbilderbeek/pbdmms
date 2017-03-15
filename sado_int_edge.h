#ifndef SADO_INT_EDGE_H
#define SADO_INT_EDGE_H

#include<boost/graph/adjacency_list.hpp>
#include "sado_indiv_graph.h"
#include "sado_species_graph.h"

namespace sado {


sado::indiv_edg_desc
add_int_edge(
    const vert_desc& vd_from,
    const vert_desc& vd_to,
    const int i,
    pedigree_graph& g
    );

int get_int_edge(const indiv_edg_desc& ed, const pedigree_graph& g) noexcept;

sado::sp_edg_desc
add_int_edge(
    const vert_desc& vd_from,
    const vert_desc& vd_to,
    const int i,
    species_graph& g
    );

int get_int_edge(const sp_edg_desc& ed, const species_graph& g) noexcept;

}//~namespace sado

#endif // SADO_INT_EDGE_H
