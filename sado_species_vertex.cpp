#include "sado_species_vertex.h"

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include "sado_ancestry_graph.h"

std::vector<sado::species> sado::get_species_vertexes(const ancestry_graph& g) noexcept
{
  std::vector<species> v;
  v.reserve(boost::num_vertices(g));
  const auto vip = vertices(g);
  std::transform(
    vip.first,
    vip.second,
    std::back_inserter(v),
    [g](const auto d)
    {
      return g[d];
    }
  );
  return v;
}
