#ifndef SADO_SPECIES_GRAPH_H
#define SADO_SPECIES_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include <vector>

namespace sado {

/// Connects species that are related by ancestry
/// Created from species by create_ancestry_graph
/// ancestor -> kid
using ancestry_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::undirectedS, //Classic, to be changed to bidirectionalS first, then ideally to directedS
  //boost::bidirectionalS, //Can have a direction, http://stackoverflow.com/questions/18791319/calculate-number-of-in-and-out-edges-in-a-boostgraph-vertex
  //boost::directedS,
  species
>;

using sp_vert_desc = boost::graph_traits<ancestry_graph>::vertex_descriptor;

using sp_edg_desc = boost::graph_traits<ancestry_graph>::edge_descriptor;

///Clear all species that have no extant descendants
///Does not remove those cleared vertices
void clear_extinct(ancestry_graph& g) noexcept;

std::vector<sp_vert_desc> collect_root_vds(const ancestry_graph& g);

///Collect all vertex descriptor that are (1) younger than the given one, and
/// (2) at a node or at a tip
std::vector<sp_vert_desc> collect_younger_nodes(
  const sp_vert_desc vd,
  const ancestry_graph& g);

ancestry_graph create_ancestry_graph(
  const std::vector<species>& species) noexcept;

ancestry_graph create_reconstructed(ancestry_graph g) noexcept;


///Creates a testing graph
/*

 0 -> .
 1 -> .
 2 -> 0
 3 -> {0, 1}
 4 -> {0, 1}
 5 -> {0, 1}
 6 -> {2, 3}
 7 -> {4, 5}

    [6]      [7]
    /\        /\
   /  \      /  \
 [2]  [3]  [4] [5]
   \   |\  /| ./
    \  | \/.| /
     \ |./\ |/
      [0]  [1]
*/
ancestry_graph create_test_graph_1() noexcept;


/*
 [2]  [3]
  |  /
  | /
 [1]
  |
  |
 [0]
*/
ancestry_graph create_test_graph_2() noexcept;

/*
 [2]
  |
  |
 [1]
  |
  |
 [0]
*/
ancestry_graph create_test_graph_3() noexcept;


///Create a species graph with a back-mutatation
/*
 [3]
  | \
  |  \
 [1]  [2]
  |  /
  | /
 [0]
*/
ancestry_graph create_test_graph_4() noexcept;

///Create a graph with an extinct species
/*
 2 + [3]
   |  |
   |  |
 1 + [1]  [2]
   |  |  /
   |  | /
 0 + [0]

 t (generations)
*/
ancestry_graph create_test_graph_5() noexcept;

///Create a graph with two generations
/*
 [1]
  |
  |
 [0]
*/
ancestry_graph create_test_graph_6() noexcept;

///Create a graph with one generations
/*

 [0]

*/
ancestry_graph create_test_graph_7() noexcept;


///Three species in the second generation, that merge back into one
///in the next generation
  /*
    [4]         [4]
   / | \         |
  /  |  \        |
 [1][2][3] ->   [1]
  \  |  /        |
   \ | /         |
    [0]         [0]
  */
ancestry_graph create_test_graph_8() noexcept;

///2 species that coexist for two generations, then merge back into one
///in the fourth generation
/*
    [5]       [5]
    /\         |
   /  \        |
  [3] [4]     [4]
  |    |       |
  |    |       |
 [1]  [2] ->  [1]
  |   /        |
  |  /         |
  [0]         [0]
  */
ancestry_graph create_test_graph_9() noexcept;

/*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  |  / |       |
  | /  |       |
 [0]  [1]     [0]
  */
ancestry_graph create_test_graph_10() noexcept;

/*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  | \  |       |
  |  \ |       |
 [0]  [1]     [0]
  */
ancestry_graph create_test_graph_11() noexcept;

/*
     [5]         [5]
    / | \         |
   /  |  \        |
 [2] [3] [4] ->  [2]
    \ | / |       |
     \|/  |       |
     [0] [1]     [0]
  */
ancestry_graph create_test_graph_12() noexcept;

/*

     [3] [4]     [3] [4]
      | / |       | /
      |/  |       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]

*/
ancestry_graph create_test_graph_13() noexcept;

/*

     [3] [4]     [3] [4]
      | \ |       | /
      |  \|       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]

*/
ancestry_graph create_test_graph_14() noexcept;

/// Two ancestors
/*

                           time

     [3]            [3]      + present, generation 2
      |              |       |
      |              |       |
     [2]      ->    [2]      + past, generation 1
      | \            |       |
      |  \           |       |
     [0] [1]        [0]      + past, generation 0
      .  .
      . .
      .
     [?]
*/
ancestry_graph create_test_graph_15() noexcept;

///Create a phylogeny in which speciation occurred
///immediatly and lasted for two generations
/*

     [3] [4]
      |   |
      |   |
     [1] [2]
      | /
      |/
     [0]
*/
ancestry_graph create_test_graph_16() noexcept;

 /*
             [8]
            / |
           /  |
         [6] [7]
        / |   |
       /  |   |
     [3] [4] [5]
      |   |  /
      |   | /
     [1] [2]
      | /
      |/
     [0]
  */
ancestry_graph create_test_graph_17() noexcept;

///Create a minimal graph with a speciation
/*

  1 +  {b} {c}
    |   |  /
    |   | /
  0 +  {a}

  t (generation)

*/
ancestry_graph create_test_graph_18() noexcept;

///Create a testing phylogeny with three species after two generations
/*

 2 +    [3]   [4]   [5]
   |      \     \   /
   |       \     \ /
 1 +       [1]   [2]
   |         \   /
   |          \ /
 0 +          [0]

   t (generation)
*/
ancestry_graph create_test_graph_19() noexcept;

/*

    [3]         [2]
   / | \         |
  /  |  \        |
 |  [1]--[2] -> [1]
  \  |  /        |
   \ | /         |
    [0]         [0]

 */
ancestry_graph create_test_graph_20() noexcept;

/*

    [2]        [2]
   / |          |
  |  |          |
  | [1]   -->  [1]
  \  |          |
   \ |          |
    [0]        [0]
 */
ancestry_graph create_test_graph_21() noexcept;

///Creates all the test graphs
std::vector<ancestry_graph> create_test_graphs() noexcept;

///Count the number of species in the present
int count_n_extant(const sado::ancestry_graph& g);

///Count the number of generations
int count_n_generations(const sado::ancestry_graph& g);

///Count the number of species in a certain generation
int count_number_species_in_generation(const sado::ancestry_graph& g, const int gen);

///Collects *all* descendants of the species at vertex descriptor vd
std::vector<species> get_descendants(const sp_vert_desc vd, const ancestry_graph& g);

int get_last_descendant_generation(const sp_vert_desc vd, const ancestry_graph& g);

///Collect all vertex descriptors in the next/younger generation
///compared to the given vertex descriptor
std::vector<sp_vert_desc> get_next_generation_vds(
  sp_vert_desc vd, const ancestry_graph& g);

///Collect all vertex descriptors in the next/younger generation
/// of all given vertex descriptors
std::vector<sp_vert_desc> get_next_generation_vds(
  const std::vector<sp_vert_desc>& vds, const ancestry_graph& g);

///Collect the species that are either one generation before or after this one
std::vector<species> get_related(const sp_vert_desc vd, const ancestry_graph& g);

///Does this species have an older relative?
///May be not if
/// (1) it is the first generation
/// (2) in the phylogeny merge/zip algorithm
bool has_ancestor(const sp_vert_desc vd, const ancestry_graph& g);

///Check if the species at the vertex descriptors share a common vertex
bool has_common_descendant(
  const sp_vert_desc vd_a, const sp_vert_desc vd_b, const ancestry_graph& g);

///Does the species at this vertex have an extant descendant?
bool has_extant_descendant(const sp_vert_desc vd, const ancestry_graph& g);

///Does the species at this vertex have an extant descendant?
bool has_extant_descendant(const sp_vert_desc vd, const ancestry_graph& g,
  const int n_generations);

//Classic
bool has_extant_descendant_impl1(const sp_vert_desc vd, const ancestry_graph& g,
  const int n_generations);
//New
bool has_extant_descendant_impl2(const sp_vert_desc vd, const ancestry_graph& g,
  const int n_generations);

///Do these vectors share a common vertex descriptor?
bool has_intersection(
  std::vector<sp_vert_desc> a, std::vector<sp_vert_desc> b) noexcept;

///Is this vertex descriptor at the end of the graph?
bool is_tip(const sp_vert_desc vd, const ancestry_graph& g);

///May species from 'from' be transferred to 'to' by the transfer_species function?
bool may_transfer(sp_vert_desc from, sp_vert_desc to, const ancestry_graph& g);

///Of two species, if
/// - they are from the same generation
/// - share a descendant
///Then
/// - Move all individuals to one of the two strands that has an ancestor
/// - Clear the vertex the individuals where moved away from
void merge_split_species(ancestry_graph& g);

///Remove all vertices without edges
void remove_cleared_vertices(ancestry_graph& g) noexcept;

///Remove the edges that span more generations
void remove_multi_generation_edges(ancestry_graph& g);

///Remove the edges that have a same source and target
void remove_self_loops(ancestry_graph& g);

///Save a graph as a .png
void save_to_png(const ancestry_graph& g, const std::string& filename);

///Transfer all edges from source to target
///In the end, all connections made with 'source' are now connected to 'target'
void transfer_connections(
  const sp_vert_desc source,
  const sp_vert_desc target,
  ancestry_graph& g
);

std::ostream& operator<<(std::ostream& os, const ancestry_graph& g) noexcept;

} //~namespace sado

#endif // SADO_SPECIES_GRAPH_H
