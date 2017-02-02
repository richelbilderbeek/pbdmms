#ifndef SADO_INT_EDGE_H
#define SADO_INT_EDGE_H

#include<boost/graph/adjacency_list.hpp>
#include<sado_indiv_graph.h>

namespace sado {


sado::edg_desc
add_int_edge(
    const vert_desc& vd_from,
    const vert_desc& vd_to,
    const int i,
    sado::indiv_graph& g
    );

int get_int_edge(const sado::edg_desc& ed, const sado::indiv_graph& g) noexcept;

}//~namespace sado

#endif // SADO_INT_EDGE_H
