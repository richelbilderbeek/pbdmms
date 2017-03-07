#include "sado_individual_vertex.h"

#include <boost/graph/adjacency_list.hpp>
#include "sado_individual.h"

sado::vert_desc sado::add_individual_vertex(const sado::indiv& i, sado::pedigree_graph& g) noexcept
{
  static_assert(!std::is_const<sado::pedigree_graph>::value, "graph cannot be const");
  const auto vd = boost::add_vertex(g);
  g[vd] = i;
  return vd;
}

std::vector<sado::indiv> sado::get_individual_vertexes(const sado::pedigree_graph& g) noexcept
{
  using vd = typename sado::pedigree_graph::vertex_descriptor;
  std::vector<sado::indiv> v(boost::num_vertices(g));
  const auto vip = vertices(g);
  std::transform(vip.first, vip.second, std::begin(v), [g](const vd& d) {return g[d];});
  return v;

}
