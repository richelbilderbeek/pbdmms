#include "sado_species_graph.h"

#include <boost/graph/adjacency_list.hpp>
#include "has_edge_between_vertices.h"
#include "sado_species.h"
#include "sado_parameters.h"
#include "sado_species_vertex.h"
#include "sado_int_edge.h"
#include "sado_individual.h"
#include "sado_id.h"

#include <vector>
#include <cassert>

sado::species_graph
create_empty_directed_species_graph() noexcept
{
  return {};
}

sado::species_graph
create_my_species_graph() noexcept
{

  auto g = create_empty_directed_species_graph();
  /*const sado::parameters p = sado::create_golden_standard_parameters();

  const sado::sado_species mother;
  const sado::sado_species father;
  const sado::sado_species kid = sado::create_offspring(mother, father, p);

  const auto vd_a = add_species_vertex(mother, g);
  const auto vd_b = add_species_vertex(father, g);
  const auto vd_c = add_species_vertex(kid, g);
  boost::add_edge(vd_a, vd_c, g);
  boost::add_edge(vd_b, vd_c, g);*/
  return g;

}

sado::species_graph create_graph_from_species_vector(const std::vector<sado::sado_species>& species) noexcept
{

  if(species.empty()) throw std::invalid_argument("Vector with species is empty");

  auto g = create_empty_directed_species_graph();

  using vertex_des = typename boost::graph_traits<sado::species_graph>::vertex_descriptor;

  std::vector<std::pair<int,vertex_des>> v;

  for(int i = 0; i != static_cast<int>(species.size()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(species.size()));
    const auto vd = add_species_vertex(species[i], g);
    const std::pair<int, vertex_des> vd_pair{i,vd};
    v.push_back(vd_pair);
  }

  /// Go through all species
  for (const std::pair<int, vertex_des> pair_i : v)
  {
    /// And all other species
    for (const std::pair<int, vertex_des> pair_j : v)
    {
      if (pair_j.first!=pair_i.first)
      {
        const auto sp_i = species[pair_i.first];
        assert(!sp_i.empty());
        const auto sp_j = species[pair_j.first];
        assert(!sp_j.empty());

        /// Go trough all indivs in species_i
        for(int k = 0; k != static_cast<int>(sp_i.size()); ++k)
        {
          assert(k >= 0);
          assert(k < static_cast<int>(sp_i.size()));

          ///And connect them to indivs in species_j
          for(int l =0; l != static_cast<int>(sp_j.size()); ++l)
          {
            assert(l >= 0);
            assert(l < static_cast<int>(sp_j.size()));
            ///If indiv i is either father or mother from indiv j
            /// and there is no edge between the species yet
            /// add edge between species.
            if (
                (sp_i[k].get_father_id() == sp_j[l].get_id()
                ||
                sp_i[k].get_mother_id() == sp_j[l].get_id())
                &&
                !has_edge_between_vertices(pair_i.second, pair_j.second,g)
                )
            {
              int generations = sp_j.get_generation() - sp_i.get_generation();
              sado::add_int_edge(pair_i.second, pair_j.second, generations, g);
            }


          }

        }


      }
    }

    /* Go through all species
     * Get all indivs from species
     * Connect indivs with their parents
     * Connect species when there is a connection between one or more of their indivs
     */
  }

  return g;
/*
  using vertex_des = typename boost::graph_traits<sado::species_graph>::vertex_descriptor;

  std::vector<std::pair<int, vertex_des>> v;
  for(int i = 0; i != static_cast<int>(species.size()); ++i)
  {
    const auto vd = add_species_vertex(species[i], g);
    const std::pair<int, vertex_des>
        vd_pair{i, vd};
    v.push_back(vd_pair);
  }

  for (const std::pair<int, vertex_des> pair : v)
  {
    for(int j = 0; j != static_cast<int>(species.size()); ++j)
    {
      if (species[pair.first].get_father_id() == species[j].get_id()
          || species[pair.first].get_mother_id() == species[j].get_id())
       sado::add_int_edge(v[j].second, pair.second, 1, g);
    }
  }
*/
  return g;
}
