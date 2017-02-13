#include "sado_int_edge.h"
#include "has_edge_between_vertices.h"

sado::edg_desc
sado::add_int_edge(
    const sado::vert_desc& vd_from,
    const sado::vert_desc& vd_to,
    const int i,
    sado::indiv_graph& g
    )
{
  static_assert(!std::is_const<indiv_graph>::value, "graph cannot be const");
  if(has_edge_between_vertices(vd_from, vd_to, g))
  {
    std::stringstream msg;
    msg << __func__ << ": already has an edge there";
    throw std::invalid_argument(msg.str());
  }
  const auto aer = boost::add_edge(vd_from, vd_to, g);
  assert(aer.second);
  g[aer. first] = i;
  return aer.first;
}

int sado::get_int_edge(const sado::edg_desc& ed, const sado::indiv_graph& g) noexcept
{
  return g[ed];
}

sado::edg_desc
sado::add_int_edge(
    const sado::vert_desc& vd_from,
    const sado::vert_desc& vd_to,
    const int i,
    sado::species_graph& g
    )
{
  static_assert(!std::is_const<species_graph>::value, "graph cannot be const");
  if(has_edge_between_vertices(vd_from, vd_to, g))
  {
    std::stringstream msg;
    msg << __func__ << ": already has an edge there";
    throw std::invalid_argument(msg.str());
  }
  const auto aer = boost::add_edge(vd_from, vd_to, g);
  assert(aer.second);
  g[aer. first] = i;
  return aer.first;
}

int sado::get_int_edge(const sado::edg_desc& ed, const sado::species_graph& g) noexcept
{
  return g[ed];
}
