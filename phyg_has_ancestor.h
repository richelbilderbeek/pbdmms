#ifndef PHYG_HAS_ANCESTOR_H
#define PHYG_HAS_ANCESTOR_H

namespace phyg {

/// Determines if a vertex/species has an ancestor.
/// Because the graph is directed from ancestor to offspring,
/// a vertex/species has an ancestor if there is an edge coming
/// (from its ancestor) to itself.
/// May be false if
///  (1) it is the first generation
///  (2) in the phylogeny merge/zip algorithm
template <class graph>
bool has_ancestor(
  const typename boost::graph_traits<graph>::vertex_descriptor vd,
  const graph& g)
{
  //If it has an in-edge
  const auto eip = boost::in_edges(vd, g);
  return eip.first != eip.second;
}

} //~namespace phyg

#endif // PHYG_HAS_ANCESTOR_H
