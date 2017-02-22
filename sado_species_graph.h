#ifndef SADO_SPECIES_GRAPH_H
#define SADO_SPECIES_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include <vector>

namespace sado {

using species_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  sado::species,
  int
>;

using sp_vert_desc = boost::graph_traits<species_graph>::vertex_descriptor;

using sp_edg_desc = boost::graph_traits<species_graph>::edge_descriptor;

///Clear all species that have no extant descendants
///Does not remove those cleared vertices
void clear_extinct(species_graph& g) noexcept;

species_graph
create_empty_directed_species_graph() noexcept;

species_graph
create_my_species_graph() noexcept;


species_graph
create_graph_from_species_vector(const std::vector<sado::species>& species) noexcept;\

species_graph
create_reconstructed(sado::species_graph g) noexcept;


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
species_graph create_test_graph_1() noexcept;


/*
 [2]  [3]
  |  /
  | /
 [1]
  |
  |
 [0]
*/
species_graph create_test_graph_2() noexcept;

/*
 [2]
  |
  |
 [1]
  |
  |
 [0]
*/
species_graph create_test_graph_3() noexcept;


/*
 [3]          [3]
  | \         |
  |  \        |
 [1]  [2] -> [1]
  |  /        |
  | /         |
 [0]         [0]
*/
species_graph create_test_graph_4() noexcept;

///Create a graph with an extinct species
/*
 [3]
  |
  |
 [1]  [2]
  |  /
  | /
 [0]
*/
species_graph create_test_graph_5() noexcept;

///Create a graph with two generations
/*
 [1]
  |
  |
 [0]
*/
species_graph create_test_graph_6() noexcept;

///Create a graph with one generations
/*

 [0]

*/
species_graph create_test_graph_7() noexcept;


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
species_graph create_test_graph_8() noexcept;

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
species_graph create_test_graph_9() noexcept;

/*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  |  / |       |
  | /  |       |
 [0]  [1]     [0]
  */
species_graph create_test_graph_10() noexcept;

/*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  | \  |       |
  |  \ |       |
 [0]  [1]     [0]
  */
species_graph create_test_graph_11() noexcept;

/*
     [5]         [5]
    / | \         |
   /  |  \        |
 [2] [3] [4] ->  [2]
    \ | / |       |
     \|/  |       |
     [0] [1]     [0]
  */
species_graph create_test_graph_12() noexcept;

/*

     [3] [4]     [3] [4]
      | / |       | /
      |/  |       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]

*/
species_graph create_test_graph_13() noexcept;

/*

     [3] [4]     [3] [4]
      | \ |       | /
      |  \|       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]

*/
species_graph create_test_graph_14() noexcept;

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
species_graph create_test_graph_15() noexcept;

/*

     [3] [4]
      |   |
      |   |
     [1] [2]
      | /
      |/
     [0]
  */
species_graph create_test_graph_16() noexcept;

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
species_graph create_test_graph_17() noexcept;


int
count_n_generations(const sado::species_graph& g);

int
count_number_species_in_generation(const sado::species_graph& g, const int gen);

///Collects *all* descendants of the species at vertex descriptor vd
std::vector<species> get_descendants(const sp_vert_desc vd, const species_graph& g);

int get_last_descendant_generation(const sp_vert_desc vd, const species_graph& g);

///Collect all vertex descriptors in the next/younger generation compared to the given vertex descriptor
std::vector<sp_vert_desc> get_next_generation_vds(sp_vert_desc vd, const species_graph& g);

///Collect all vertex descriptors in the next/younger generation of all given vertex descriptors
std::vector<sp_vert_desc> get_next_generation_vds(const std::vector<sp_vert_desc>& vds, const species_graph& g);

///Collect the species that are either one generation before or after this one
std::vector<species> get_related(const sp_vert_desc vd, const species_graph& g);

///Does this species have an older relative?
///May be not if
/// (1) it is the first generation
/// (2) in the phylogeny merge/zip algorithm
bool has_ancestor(const sp_vert_desc vd, const species_graph& g);

///Check if the species at the vertex descriptors share a common vertex
bool has_common_descendant(const sp_vert_desc vd_a, const sp_vert_desc vd_b, const species_graph& g);

///Does the species at this vertex have an extant descendant?
bool has_extant_descendant(const sp_vert_desc vd, const species_graph& g);

///Do these vectors share a common vertex descriptor?
bool has_intersection(std::vector<sp_vert_desc> a, std::vector<sp_vert_desc> b) noexcept;

///Remove all vertices without edges
void remove_cleared_vertices(species_graph& g) noexcept;

///Save a graph as a .png
void save_to_png(const species_graph& g, const std::string& filename);

///Transfer all edges from source to target
///In the end, all connections made with 'source' are now connected to 'target'
void transfer_connections(
  const sp_vert_desc source,
  const sp_vert_desc target,
  species_graph& g
);

std::ostream& operator<<(std::ostream& os, const species_graph& g) noexcept;

} //~namespace sado

#endif // SADO_SPECIES_GRAPH_H
