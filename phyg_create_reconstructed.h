#ifndef PHYG_CREATE_RECONSTRUCTED_H
#define PHYG_CREATE_RECONSTRUCTED_H

namespace phyg {

///From a complete phylogeny, remove the species that have no
///representatives in the present
template<class ancestry_graph>
ancestry_graph create_reconstructed(const ancestry_graph& g)
{
  if (boost::num_vertices(g) <= 1)
  {
    return g;
  }
  /*

  //Remove the edges that span more generations
  //std::cerr << "Remove the edges that span more generations\n";
  remove_multi_generation_edges(g, interval);

  //Clear all species that have no extant descendants
  //std::cerr << "Clear all species that have no extant descendants\n";
  clear_extinct(g);

  //Remove all unconnected vertices
  //std::cerr << "Remove all unconnected vertices\n";
  remove_cleared_vertices(g);

  //merge split species by transferring individuals
  //std::cerr << "merge split species by transferring individuals\n";
  merge_split_species(g);

  //Remove all unconnected vertices
  //std::cerr << "Remove all unconnected vertices\n";
  remove_cleared_vertices(g);

  //Remove the edges that have a same source and target
  //std::cerr << "Remove the edges that have a same source and target\n";
  remove_self_loops(g);


  */
  return g;
}

} //~namespace phyg

#endif // PHYG_CREATE_RECONSTRUCTED_H
