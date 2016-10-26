#ifndef RESULTS_H
#define RESULTS_H

#include <iosfwd>
#include <vector>
#include "ribi_individual.h"
#include "ribi_sil_frequency_phylogeny.h"
#include "ribi_population.h"
#include "ribi_parameters.h"

namespace ribi {

class results
{
public:
  results(
    const int max_genetic_distance
  );

  ///Measure the population at time t
  void add_measurement(
    const int t,
    const population& any_population
  );

  int get_max_genetic_distance() const noexcept
  {
    return m_max_genetic_distance;
  }

  sil_frequency_phylogeny get_sil_frequency_phylogeny() const noexcept
  {
    return m_sil_frequency_phylogeny;
  }

  ///Call 'summarize_sil_frequency_phylogeny' first
  sil_frequency_phylogeny get_summarized_sil_frequency_phylogeny() const noexcept
  {
    return m_summarized_sil_frequency_phylogeny;
  }

  ///Creates the summarized SIL frequency phylogeny
  void summarize_sil_frequency_phylogeny(bool verbose = false);

  ///Just save everything
  ///This is non-const as the summarization of the data takes place then
  ///Can only be done before summarize_sil_frequency_phylogeny
  void save_all(const std::string& dot_filename);

  ///Saves the current data
  ///Usually, you will want to call 'summarize_sil_frequency_phylogeny'
  ///first
  void save(const std::string& dot_filename) const;

private:
  int m_max_genetic_distance;

  sil_frequency_phylogeny m_sil_frequency_phylogeny;

  ///will be created by 'get_summarized_sil_frequency_phylogeny'
  sil_frequency_phylogeny m_summarized_sil_frequency_phylogeny;

  ///The vertex descriptors of the previous generation
  std::vector<sil_frequency_vertex_descriptor> m_vds_prev;

  friend std::ostream& operator<<(std::ostream& os, const results& r) noexcept;
};

///The SILs of this latest generation have been tallied,
///and are added to the graph here. The vertex descriptors
///are returned to work with
std::vector<sil_frequency_vertex_descriptor> add_sils(
  const std::map<sil,int>& m,
  const int t,
  sil_frequency_phylogeny& g
) noexcept;

///Measures if all vds have the same ID
bool all_vds_have_same_id(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept;


///vds are collected per cohort. This function checks if indeed
///they have are from the same time
bool all_vds_have_same_time(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept;

///These vds have been collected when adding new genotypes.
///The vertex descriptors should each point to a vertex with a unique SIL
bool all_vds_have_unique_sil(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept;

///Clears all genotype frequencies
void clear_all_sil_frequencies(
  sil_frequency_phylogeny& g
) noexcept;

///Clear (but do not remove) a vertex with a certain ID
void clear_vertex_with_id(
  const int id,
  sil_frequency_phylogeny& g
);

///Connect the vertices of SILs that can create viable offspring together
void connect_species_within_cohort(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const int max_genetic_distance,
  sil_frequency_phylogeny& g
) noexcept;

///Connect the vertices of SILs that can create viable offspring together
void connect_species_between_cohorts(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const std::vector<sil_frequency_vertex_descriptor>& vds_prev,
  const int max_genetic_distance,
  sil_frequency_phylogeny& g
) noexcept;

void connect_vertices_with_ids(
  const int id_a, const int id_b,
  const sil_frequency_edge& edge,
  sil_frequency_phylogeny& g
);

///Count the total number of SILs these vds point to
int count_sils(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept;


///Find the common ancestor of the given vertex descriptors
///assumes the vds are from the same point in time
sil_frequency_vertex_descriptor find_common_ancestor(
  sil_frequency_vertex_descriptors vds,
  const sil_frequency_phylogeny& g
);



///Finds the splits and mergers in a graph.
/*
For example:

              3--5
             /    \
 past-0--1--2      7--8--9--present
             \    /
              4--6


will return { {7,2} }
*/
/// @return {split, merge}s (as seen from present)
sil_frequency_vertex_descriptor_pairs find_splits_and_mergers(
  sil_frequency_phylogeny& g
) noexcept;

sil_frequency_vertex_descriptor_pairs find_splits_and_mergers_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
) noexcept;

///Fuse vertices with same SIL frequencies connected
///over a timespan
/*

          1   1   1
   Past 0---1---2---3 Present

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{000,1},{001,1}}
   2 | 3 | {{000,1},{001,1}}
   3 | 4 | {{001,2}}
   --+---+-------------------

          1   2
   Past 0---1---3 Present

*/
void fuse_vertices_with_same_sil_frequencies(
  sil_frequency_phylogeny& g
);


///Fuse vertices with same SIL frequencies connected
///over a timespan, from here
void fuse_vertices_with_same_sil_frequencies_once_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
) noexcept;

///Fuse vertices with same SIL frequencies connected
///over a timespan, from here, to there
void fuse_vertices_with_same_sil_frequencies_once_from_here_via_there(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  sil_frequency_phylogeny& g
) noexcept;

///Fuse vertices with same SIL frequencies connected
///over a timespan
void fuse_vertices_with_same_sil_frequencies(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  const sil_frequency_vertex_descriptor next_neighbor,
  sil_frequency_phylogeny& g
);

///Fuse the vertices with the same style
///
///For example:
///
///   1   1            2
/// A---B---C  ->  A-------C
///
///                    B
///
/// B will be disconnected
void fuse_vertices_with_same_style(
  sil_frequency_phylogeny& g
) noexcept;

///Fuses a chain of the vertices arranged like
/// 'vd -> neighbor -> next_neighbor' to
/// 'vd -------------> next_neighbor'.
/// @param vd the focal vertex
/// @param neighbor a neighbor of the focal vertex, only has two neighbors
/// @param next_neighbor a neighbor of the neighbor
/// @param g a graph
void fuse_vertices_with_same_style(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  const sil_frequency_vertex_descriptor next_neighbor,
  sil_frequency_phylogeny& g
);

///Fuse the first suitable vertices occurence found with the same style
///starting at the vertex with vertex descriptor 'vd'
///
///For example:
///
///   1   1            2
/// A---B---C  ->  A-------C
///
///                    B
///
/// A is described by vd
/// B will be disconnected
void fuse_vertices_with_same_style_once_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
) noexcept;

///Fuse the first suitable vertices occurence found with the same style
///starting at the vertex with vertex descriptor 'vd'
///
///For example:
///
///   1   1            2
/// A---B---C  ->  A-------C
///
///                    B
///
/// A is described by vd
/// B is the neighbor, will be disconnected
/// @return if there has been a suitable vertex combination
void fuse_vertices_with_same_style_once_from_here_via_there(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  sil_frequency_phylogeny& g
) noexcept;

///Implementation of how to summarize a SIL frequency phylogeny
sil_frequency_phylogeny get_summarized_sil_frequency_phylogeny(
  const sil_frequency_phylogeny& g
);

/// Obtain the vertex descriptors of older vertices
/// (vertices with a lower generation number)
sil_frequency_vertex_descriptors get_older(
  sil_frequency_vertex_descriptor vd,
  const sil_frequency_phylogeny& g
);

/// Obtain the vertex descriptors of older vertices
/// (vertices with a lower generation number)
sil_frequency_vertex_descriptors get_older(
  sil_frequency_vertex_descriptors vds,
  const sil_frequency_phylogeny& g
);

/// Obtain the vertex descriptors of older vertices
/// (vertices with a lower generation number)
sil_frequency_vertex_descriptors get_younger(
  sil_frequency_vertex_descriptor vd,
  const sil_frequency_phylogeny& g
);

/// Obtain the vertex descriptors of older vertices
/// (vertices with a lower generation number)
sil_frequency_vertex_descriptors get_younger(
  sil_frequency_vertex_descriptors vds,
  const sil_frequency_phylogeny& g
);

///Remove vertices with zero genotypes and no connections
void remove_unconnected_empty_vertices(
  sil_frequency_phylogeny& g
) noexcept;

///Clear and remove a vertex with a certain ID
void remove_vertex_with_id(
  const int id,
  sil_frequency_phylogeny& g
);

///Determine the style of the vertices: incipient or good?
///The results depents on the genotype pool at each timepoint
///and the genetic distance needed to prevent mating.
///For example, when the genotypes are '00', '01' and '11'
///  and the max_genetic_distance is 1, the style will become
///  'incipinient', as there is the *potential* of getting
///  multiple species (as there will be two species when '01'
///  gets lost). For a genetic distance of 2, the style
///  will be 'good'
void set_all_vertices_styles(
  sil_frequency_phylogeny& g,
  const int max_genetic_distance
) noexcept;

///bs: before summary
std::string get_filename_bs_dot(const std::string& user_filename) noexcept;

///bs: before summary
std::string get_filename_bs_png(const std::string& user_filename) noexcept;

///bs: before summary
std::string get_filename_bs_svg(const std::string& user_filename) noexcept;

std::string get_filename_dot(const std::string& user_filename) noexcept;
std::string get_filename_png(const std::string& user_filename) noexcept;
std::string get_filename_svg(const std::string& user_filename) noexcept;

///Will summarize the sil_frequency_phylogeny, to be run after a simulation,
///as it used the last vertex descriptors
///(?but perhaps it can be called during a sim?)
///After 'summarize', call 'get_sil_frequency_phylogeny' again
/*

  A --- B ---- C -> A     B --- C
  |                       |
  |                       |
  D                       D

  Where timepoints of A, B and D are equal

*/
void summarize_genotypes(sil_frequency_phylogeny& g);

///Implementation of summarize_genotypes for
///a specific vertex descriptor
void summarize_genotypes_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
);

///Zip the vertices with the same style
/*
For example:

         3--5
        /    \
 0--1--2      7--8--9
        \    /
         4--6

 Should become

 0--1--2==A==B==7--8--9

*/
void zip(
  sil_frequency_phylogeny& g
) noexcept;

///Implementation of zip
///Zip with the splits and mergers known
void zip(
  const sil_frequency_vertex_descriptor_pairs& splits_and_mergers,
  sil_frequency_phylogeny& g
) noexcept;

///Implementation of zip
///Zip from the split to the merger
void zip(
  const sil_frequency_vertex_descriptor_pair& split_and_merger,
  sil_frequency_phylogeny& g
) noexcept;

} //~namespace ribi

#endif // RESULTS_H
