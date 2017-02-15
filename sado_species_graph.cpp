#include "sado_species_graph.h"

#include <boost/graph/adjacency_list.hpp>
#include "has_edge_between_vertices.h"
#include "get_vertex_out_degrees.h"
#include "sado_species.h"
#include "sado_parameters.h"
#include "sado_species_vertex.h"
#include "sado_int_edge.h"
#include "sado_individual.h"
#include "sado_id.h"

#include <vector>
#include <cassert>

sado::species_graph
sado::create_empty_directed_species_graph() noexcept
{
  return {};
}

sado::species_graph
sado::create_my_species_graph() noexcept
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

sado::species_graph sado::create_graph_from_species_vector(const std::vector<sado::sado_species>& species) noexcept
{


  if(species.empty()) throw std::invalid_argument("Vector with species is empty");

  auto g = create_empty_directed_species_graph();

  using vertex_des = typename boost::graph_traits<sado::species_graph>::vertex_descriptor;
  using sp_vd_pair = typename std::pair<sado::sado_species, vertex_des>;

  std::vector<sp_vd_pair> v;

  for(int i = 0; i != static_cast<int>(species.size()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(species.size()));
    const auto vd = add_species_vertex(species[i], g);
    const sp_vd_pair vd_pair{species[i],vd};
    v.push_back(vd_pair);
  }

  /// Go through all species
  for (const sp_vd_pair pair_i : v)
  {
    /// And all other species
    for (const sp_vd_pair pair_j : v)
    {
      if (pair_j.first!=pair_i.first)
      {
        const auto sp_i = pair_i.first;
        assert(!sp_i.empty());
        const auto sp_j = pair_j.first;
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
  }

    /* Go through all species
     * Get all indivs from species
     * Connect indivs with their parents
     * Connect species when there is a connection between one or more of their indivs
     * Go through all species top (last gen) to bottom (first gen) and merge splits.
     *
     */

  ///Can become seperate function

  ///number of generations
  const int num_gen = species.back().get_generation();

  using gen_sp = typename std::vector<sp_vd_pair>;

  ///empty vectors as placeholders
  gen_sp temp_gen;
  std::vector<gen_sp> generations(num_gen, temp_gen);



  ///put all species from 1 generation in right vector.
  for (int i = 0; i != static_cast<int>(v.size()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(v.size()));
    assert(v[i].first.get_generation() - 1 >=0);
    assert(v[i].first.get_generation() - 1 < static_cast<int>(generations.size()));
    generations[v[i].first.get_generation() - 1].push_back(v[i]);
  }



  std::vector<gen_sp> fixed_generations(num_gen,temp_gen);

  for (const sp_vd_pair sp : generations.back())
  {
    assert(num_gen - 1 >= 0);
    assert(num_gen - 1 < static_cast<int>(fixed_generations.size()));
    fixed_generations[num_gen - 1].push_back(sp);
  }

  //const int out_edges =  static_cast<int>(out_degree(sp_p.second, g));

  ///starting at first to last generation
  for (int i = num_gen - 2; i != 0; --i)
  {
    ///create a vector potential new parentspecies
    sado::sado_species temp_species(i);
    vertex_des temp_vd;
    sp_vd_pair temp_pair {temp_species, temp_vd};
    assert(i+1 >= 0);
    assert(i+1 < static_cast<int>(fixed_generations.size()));
    gen_sp temp_spp(fixed_generations[i+1].size(),temp_pair);

    /// every species in this generation
    assert(i >= 0);
    assert(i < static_cast<int>(generations.size()));
    assert(!generations[i].empty());
    for (const sp_vd_pair sp : generations[i])
    {
      /// check all the potential parents for a connection with the species sp
      assert(i + 1 >= 0);
      assert(i + 1 < static_cast<int>(generations.size()));
      std::cout << "Checking every species in generation " << i << '\n';
      for (int j = 0; j != static_cast<int>(generations[i+1].size()); ++j)
      {
        assert(i + 1>= 0);
        assert(i + 1< static_cast<int>(generations.size()));
        assert(j>= 0);
        assert(j< static_cast<int>(generations[i+1].size()));
        const sp_vd_pair sp2 = generations[i+1][j];
        /// if it finds a connection keep the parent sp2
        /// add the species to the right combined species (temp_spp has all of them)
        std::cout << "Checking for edges.\n";
        if(has_edge_between_vertices(sp2.second, sp.second, g))
        {
          std::cout << "These two have an edge between them.\n";

          ///add all individuals from that species to the combined species
          for (const sado::indiv individual : sp.first.get_indivs())
          {
            std::cout << "Going through all individuals from species sp.\n";
            std::cout << "I'm at individual: " << static_cast<int>(individual.get_id()) << '\n';
            assert(j >= 0);
            assert(j < static_cast<int>(temp_spp.size()));
            temp_spp[j].first.add_indiv(individual);
            std::cout << j << '\n';
            assert(!temp_spp[j].first.empty());
            temp_spp[j].second = sp.second;
          }
        }
      }
    }
    /// push all the combined species of this generation to the fixed_generations
    int k = 0;
    for (const auto spec : temp_spp)
    {
      std::cout << ++k << '\n';
      assert(!spec.first.empty());

    }

    fixed_generations.push_back(temp_spp);
  }


  auto h = create_empty_directed_species_graph();

  std::vector<sp_vd_pair> fixed_species;

  for(const auto gen : fixed_generations)
  {
    for (const auto spp : gen)
      fixed_species.push_back(spp);
  }

  std::vector<sp_vd_pair> w;
  w.reserve(fixed_species.size());

  if(fixed_species.empty()) throw std::invalid_argument("Vector with fixed_species is empty");

  for(int i = 0; i != static_cast<int>(fixed_species.size()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(fixed_species.size()));
    const auto vd = add_species_vertex(fixed_species[i].first, h);
    const sp_vd_pair vd_pair{fixed_species[i].first,vd};
    std::cout << i << '\n';
    assert(!vd_pair.first.empty());
    w.push_back(vd_pair);
  }

  /// Go through all species
  for (const sp_vd_pair pair_i : w)
  {
    /// And all other species
    for (const sp_vd_pair pair_j : w)
    {
      if (pair_j.first!=pair_i.first)
      {
        const auto sp_i = pair_i.first;
        assert(!sp_i.empty());
        const auto sp_j = pair_j.first;
        std::cout << "sp_j size: " << static_cast<int>(sp_j.size()) << '\n';
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
                !has_edge_between_vertices(pair_i.second, pair_j.second,h)
                )
            {
              int generations_h = sp_j.get_generation() - sp_i.get_generation();
              sado::add_int_edge(pair_i.second, pair_j.second, generations_h, h);
            }
          }
        }
      }
    }
  }

  /// I still have all vertex descriptors in the vector v (pairs of vd and number)
  ///Go through species top to bottom.

  ///Merge all species 1 species has outgoing arrows to into 1 species
  ///output new graph
  return h;
}
