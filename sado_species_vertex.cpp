#include "sado_species_vertex.h"

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include "sado_species_graph.h"

sado::sp_vert_desc sado::add_species_vertex(const sado::sado_species& s, sado::species_graph& g) noexcept
{
  static_assert(!std::is_const<sado::species_graph>::value, "graph cannot be const");
  const auto vd = boost::add_vertex(g);
  g[vd] = s;
  return vd;
}

std::vector<sado::sado_species> sado::get_species_vertexes(const sado::species_graph& g) noexcept
{
  using vd = typename sado::species_graph::vertex_descriptor;
  std::vector<sado::sado_species> v(boost::num_vertices(g));
  const auto vip = vertices(g);
  std::transform(vip.first, vip.second, std::begin(v), [g](const vd& d) {return g[d];});
  return v;

}
