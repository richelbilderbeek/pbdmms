#include "distancer_sil_frequency_phylogeny.h"
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include "distancer_sil_frequency_vertex_writer.h"

std::ostream& operator<<(std::ostream& os, const sil_frequency_phylogeny& g) noexcept
{
  boost::write_graphviz(os, g,
    sil_frequency_vertex_writer<sil_frequency_phylogeny>(g),
    sil_frequency_edge_writer<sil_frequency_phylogeny>(g)
  );
  return os;
}
