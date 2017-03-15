#ifndef SADO_SPECIES_GRAPH_VERTEX_WRITER_H
#define SADO_SPECIES_GRAPH_VERTEX_WRITER_H

#include <cassert>
#include <sstream>
#include <ostream>

#include "sado_species_graph.h"

namespace sado {

template <
  typename graph
>
class species_graph_vertex_writer {
public:
  species_graph_vertex_writer(graph g) : m_g{g}
  {

  }
  template <class vertex_descriptor>
  void operator()(
    std::ostream& out,
    const vertex_descriptor& vd
  ) const noexcept
  {
    out
      << "["
      << "label=\""
      << m_g[vd]
      << "\""
    ;
    out << "]";
  }
private:
  graph m_g;
};

} //~namespace sado


#endif // SADO_SPECIES_GRAPH_VERTEX_WRITER_H
