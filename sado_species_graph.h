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

///Does the species at this vertex have an extant descendant?
bool has_extant_descendant(const sp_vert_desc vd, const species_graph& g);

species_graph
create_empty_directed_species_graph() noexcept;

species_graph
create_my_species_graph() noexcept;


species_graph
create_graph_from_species_vector(const std::vector<sado::species>& species) noexcept;\

species_graph
create_reconstructed_graph_from_species_graph(sado::species_graph g) noexcept;


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
 [2]
  |
  |
 [1]
  |
  |
 [0]
*/
species_graph create_test_graph_2() noexcept;

/*
 [2]  [3]
  |  /
  | /
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


int
count_n_generations(const sado::species_graph& g);

int
count_number_reconstructed_species_in_generation(const sado::species_graph& g, const int gen);

///Collects *all* descendants of the species at vertex descriptor vd
std::vector<species> get_descendants(const sp_vert_desc vd, const species_graph& g);

int get_last_descendant_generation(const sp_vert_desc vd, const species_graph& g);

///Collect all vertex descriptors in the next/younger generation compared to the given vertex descriptor
std::vector<sp_vert_desc> get_next_generation_vds(sp_vert_desc vd, const species_graph& g);

///Collect all vertex descriptors in the next/younger generation of all given vertex descriptors
std::vector<sp_vert_desc> get_next_generation_vds(const std::vector<sp_vert_desc>& vds, const species_graph& g);




///Collect the species that are either one generation before or after this one
std::vector<species> get_related(const sp_vert_desc vd, const species_graph& g);

bool has_extant_descendant(const sp_vert_desc vd, const species_graph& g);

} //~namespace sado

#endif // SADO_SPECIES_GRAPH_H
