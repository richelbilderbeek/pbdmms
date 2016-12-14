#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/dynamic_bitset.hpp>
#include "ribi_dna.h"
#include "count_undirected_graph_connected_components.h"
#include "remove_nth_vertex.h"
namespace ribi {

///Checks if all three elements are different
template <class T>
bool all_different(const T& a, const T& b, const T& c)
{
  return a != b && a != c && b != c;
}

///Checks if all three elements are the same
template <class T>
bool all_same(const T& a, const T& b, const T& c)
{
  return a == b && b == c;
}

///Convert the main function is argument to a std::vector<std::string>
std::vector<std::string> convert_arguments(
  const int argc, const char * const argv[]
) noexcept;

///Counts the number of components in a graph
///For example: A-B C-D would a graph of two edges, four vertices and two connected components
///The value returned would be 2
///
template <typename graph>
int count_connected_components(
  const graph& g
) noexcept
{
  return count_undirected_graph_connected_components(g);
}

///Counts the number of bits that are different
int count_different_bits(
  const boost::dynamic_bitset<>& a,
  const boost::dynamic_bitset<>& b
);

///Create a bitset that has its bits inherited from its
///ancestors p and q.
///The 'inherit_from_p' determines per bit if that locus is inherited
///from ancestor p.
boost::dynamic_bitset<> create_offspring(
  const boost::dynamic_bitset<>& p,
  const boost::dynamic_bitset<>& q,
  const boost::dynamic_bitset<>& inherit_from_p
);

///Create a sequence that has its nucleotides inherited from its
///ancestors p and q.
///The 'inherit_from_p' determines per site if that nucleotide is inherited
///from ancestor p.
dna create_offspring(
  const dna& p,
  const dna& q,
  const boost::dynamic_bitset<>& inherit_from_p
);

///Get the ID of each node in a graph, which differs per connected component
///For example: A-B C-D would a graph of two edges, four vertices and two connected component
///The IDs returned would be {0,0,1,1}
template <typename graph>
std::vector<int> get_connected_components_ids(
  const graph& g
) noexcept
{
  std::vector<int> c(boost::num_vertices(g));
  boost::connected_components(g,
    boost::make_iterator_property_map(
      std::begin(c),
      get(boost::vertex_index, g)
    )
  );
  return c;
}

///Obtains n indices in range [0,sz>
///If n > sz, all indices will be returned
/// @param sz size of an indexed container
/// @param n number of unique indices
std::vector<std::size_t> get_unique_indices(
  const std::size_t sz,
  const int n,
  std::mt19937& rng_engine
);




} //~namespace ribi

#endif // HELPER_H
