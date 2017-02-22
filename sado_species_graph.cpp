#include "sado_species_graph.h"

#include <boost/graph/adjacency_list.hpp>

#include "has_edge_between_vertices.h"
#include "get_vertex_out_degrees.h"
#include "newick.h"
#include "sado_species.h"
#include "sado_parameters.h"
#include "sado_species_vertex.h"
#include "sado_int_edge.h"
#include "sado_individual.h"
#include "sado_id.h"
#include "save_graph_to_dot.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "sado_species_graph_vertex_writer.h"
#include <vector>
#include <cassert>

void sado::clear_extinct(sado::species_graph& g) noexcept
{
  const auto vs = vertices(g);
  for (auto vd = vs.first; vd != vs.second; ++vd)
  {
    if (!has_extant_descendant(*vd, g))
    {
      boost::clear_vertex(*vd, g);
    }
  }
}

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

sado::species_graph sado::create_graph_from_species_vector(const std::vector<sado::species>& species) noexcept

{


  if(species.empty()) throw std::invalid_argument("Vector with species is empty");

  auto g = create_empty_directed_species_graph();

  using vertex_des = typename boost::graph_traits<sado::species_graph>::vertex_descriptor;
  using sp_vd_pair = typename std::pair<sado::species, vertex_des>;

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
     *

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



  std::vector<gen_sp> reconstructed_generations(num_gen,temp_gen);
  gen_sp first_temp_spp;

    for (const sp_vd_pair sp : generations.back())
  {
    assert(num_gen -1 >= 0);
    assert(num_gen -1 < static_cast<int>(reconstructed_generations.size()));
    first_temp_spp.push_back(sp);
  }
  reconstructed_generations[num_gen - 1] = first_temp_spp;

  assert(static_cast<int>(reconstructed_generations[num_gen -1].size()) > 0);
  //const int out_edges =  static_cast<int>(out_degree(sp_p.second, g));

  ///starting at first to last generation
  for (int i = num_gen - 2; i != -1; --i)
  {
    ///create a vector potential new parentspecies
    sado::sado_species temp_species(i);
    vertex_des temp_vd;
    sp_vd_pair temp_pair {temp_species, temp_vd};
    assert(i+1 >= 0);
    assert(i+1 < static_cast<int>(reconstructed_generations.size()));
    assert(static_cast<int>(reconstructed_generations[i + 1].size()) != 0);
    gen_sp temp_spp(reconstructed_generations[i+1].size(),temp_pair);
    for (int j = 0; j != static_cast<int>(temp_spp.size()); ++j)
    {
       std::cout << "I have room for " << static_cast<int>(temp_spp[j].first.size()) << " indivs.\n";
    }

    assert(0 < static_cast<int>(temp_spp.size()));

    /// every species in this generation
    assert(i >= 0);
    assert(i < static_cast<int>(generations.size()));
    assert(!generations[i].empty());
    for (const sp_vd_pair sp : generations[i])
    {
      /// check all the potential parents for a connection with the species sp
      assert(i + 1 >= 0);
      assert(i + 1 < static_cast<int>(generations.size()));
      for (int j = 0; j != static_cast<int>(reconstructed_generations[i+1].size()); ++j)
      {
        assert(i + 1>= 0);
        assert(i + 1< static_cast<int>(generations.size()));
        assert(j>= 0);
        assert(j< static_cast<int>(generations[i+1].size()));
        const sp_vd_pair sp2 = reconstructed_generations[i+1][j];
        /// if it finds a connection keep the parent sp2
        /// add the species to the right combined species (temp_spp has all of them)
        if(has_edge_between_vertices(sp2.second, sp.second, g))
        {

          ///add all individuals from that species to the combined species
          for (const sado::indiv individual : sp.first.get_indivs())
          {
            bool indiv_seen = false;
            for (const sp_vd_pair speci : temp_spp)
            {
              const auto indivs = speci.first.get_indivs();
              for (const auto indi : indivs)
                if (individual.get_id() == indi.get_id())
                  indiv_seen = true;
            }
            if(!indiv_seen)
            {
            assert(j >= 0);
            assert(j < static_cast<int>(temp_spp.size()));
            temp_spp[j].first.add_indiv(individual);
            assert(!temp_spp[j].first.empty());
            temp_spp[j].second = sp.second;
            }
          }
        }
      }
    }
    /// push all the combined species of this generation to the reconstructed_generations
    for (const auto spec : temp_spp)
    {

      assert(!spec.first.empty());
      std::cout << "Num of indivs in sp of generation " << i << ": " << spec.first.size() << '\n';
    }
    assert(!temp_spp.empty());
    assert(i >= 0);
    assert(i < static_cast<int>(reconstructed_generations.size()));

    reconstructed_generations[i] = temp_spp;
  }
  
  auto h = create_empty_directed_species_graph();
  std::vector<sp_vd_pair> reconstructed_species;
  
  for(const auto gen : reconstructed_generations)
  {
    for (const auto spp : gen)
    {
      reconstructed_species.push_back(spp);
    }
  }

  std::vector<sp_vd_pair> w;
  w.reserve(reconstructed_species.size());

  if(reconstructed_species.empty()) throw std::invalid_argument("Vector with reconstructed_species is empty");

  for(int i = 0; i != static_cast<int>(reconstructed_species.size()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(reconstructed_species.size()));
    const auto vd = add_species_vertex(reconstructed_species[i].first, h);
    const sp_vd_pair vd_pair{reconstructed_species[i].first,vd};
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
  ///output new graph*/
  return g;
}

sado::species_graph sado::create_reconstructed(sado::species_graph g) noexcept
{
  if (boost::num_vertices(g) == 1)
  {
    return g;
  }

  clear_extinct(g);

  if (boost::num_vertices(g) > 1)
  {
    remove_cleared_vertices(g);
  }


  remove_cleared_vertices(g);

  //merge split species

  //Of two species, if
  // - they are from the same generation
  // - share a descendant
  //Then
  // - Move all individuals to one of the two strands that has an ancestor
  // - Clear the vertex the individuals where moved away from

  /*
    t (generations after start

    0    A           A          A          A
    |   / \         /          /          /
    1  B   C       B<--.      B   .      B   .
    |  |   |  ->   |      ->  |      ->  |
    2  D   E       D   E      D<--.      D   .
    |   \ /         \ /        \          \
    3    F           F          F          F



  */
  const auto vip = vertices(g);
  for (auto vi_leading = vip.first; vi_leading != vip.second; ++vi_leading)
  {
    //A leading vertex iterator has an ancestor by definition
    //or it *is* the ancestor in generation zero
    if (!has_ancestor(*vi_leading, g) && g[*vi_leading].get_generation() != 0) continue;

    for (auto vi_lagging = vip.first; vi_lagging != vip.second; ++vi_lagging)
    {
      //Cannot move to self
      if (*vi_leading == *vi_lagging) continue;
      //Cannot move between generations
      if (g[*vi_leading].get_generation() != g[*vi_lagging].get_generation()) continue;
      //Must share a descent to merge
      assert(*vi_leading != *vi_lagging);
      if (!has_common_descendant(*vi_leading, *vi_lagging, g)) continue;

      //Move the species from the lagging to the leading strand
      transfer_individuals(g[*vi_lagging], g[*vi_leading]);

      //Transfer the connections
      transfer_connections(*vi_lagging, *vi_leading, g);
      assert(boost::degree(*vi_lagging, g) == 0);
      assert(boost::degree(*vi_leading, g) > 0);

      //Disconnect the vertex
      boost::clear_vertex(*vi_lagging, g);
    }
  }

  if (boost::num_vertices(g) > 1)
  {
    remove_cleared_vertices(g);
  }

  return g;
}

sado::species_graph sado::create_test_graph_1() noexcept
{
  std::vector<species> spp;

  const indiv i;
  const indiv j;
  const indiv k;
  const indiv l;

  species first_species(0);
  species second_species(0);

  first_species.add_indiv(i);
  first_species.add_indiv(j);
  second_species.add_indiv(k);
  second_species.add_indiv(l);

  const auto p = create_article_parameters();
  const indiv kid1 = create_offspring(i,j,p);
  const indiv kid2 = create_offspring(i,k,p);
  const indiv kid3 = create_offspring(j,k,p);
  const indiv kid4 = create_offspring(i,j,p);
  const indiv kid5 = create_offspring(k,l,p);


  species third_species(1);
  species fourth_species(1);
  species fifth_species(1);
  species sixth_species(1);

  third_species.add_indiv(kid1);
  fourth_species.add_indiv(kid2);
  fifth_species.add_indiv(kid3);
  sixth_species.add_indiv(kid4);
  sixth_species.add_indiv(kid5);

  const indiv kidkid1 = create_offspring(kid1, kid2, p);
  const indiv kidkid2 = create_offspring(kid3,kid4,p);

  species seventh_species(2);
  species eighth_species(2);

  seventh_species.add_indiv(kidkid1);
  eighth_species.add_indiv(kidkid2);

  spp.push_back(first_species);
  spp.push_back(second_species);
  spp.push_back(third_species);
  spp.push_back(fourth_species);
  spp.push_back(fifth_species);
  spp.push_back(sixth_species);
  spp.push_back(seventh_species);
  spp.push_back(eighth_species);

  return create_graph_from_species_vector(spp);
}


sado::species_graph sado::create_test_graph_2() noexcept
{
  /*
   [2]  [3]
    |  /
    | /
   [1]
    |
    |
   [0]
  */

  std::vector<species> spp;

  const indiv i;
  const indiv j;

  species first_species(0);

  first_species.add_indiv(i);
  first_species.add_indiv(j);

  const auto p = create_article_parameters();
  const indiv kid1 = create_offspring(i,j,p);
  const indiv kid2 = create_offspring(i,j,p);

  species second_species(1);

  second_species.add_indiv(kid1);
  second_species.add_indiv(kid2);

  species third_species(2);
  species fourth_species(2);

  const indiv kidkid1 = create_offspring(kid1, kid2, p);
  const indiv kidkid2 = create_offspring(kid1, kid2, p);


  third_species.add_indiv(kidkid1);
  fourth_species.add_indiv(kidkid2);

  spp.push_back(first_species);
  spp.push_back(second_species);
  spp.push_back(third_species);
  spp.push_back(fourth_species);

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_3() noexcept
{
  /*
   [2]
    |
    |
   [1]
    |
    |
   [0]
  */

  const auto p = create_article_parameters();

  const indiv i;
  const indiv j;
  const indiv kid1 = create_offspring(i,j,p);
  const indiv kid2 = create_offspring(i,j,p);
  const indiv kidkid1 = create_offspring(kid1, kid2, p);

  const species first_species(0, { i, j } );
  const species second_species(1, { kid1, kid2 } );
  const species third_species(2, { kidkid1 } );

  return create_graph_from_species_vector( { first_species, second_species, third_species } );
}

sado::species_graph sado::create_test_graph_4() noexcept
{
  /*
   [3]          [3]
    | \         |
    |  \        |
   [1]  [2] -> [1]
    |  /        |
    | /         |
   [0]         [0]
  */

  std::vector<species> spp;

  const indiv grandfather;

  species first_species(0);

  first_species.add_indiv(grandfather);

  const auto p = create_article_parameters();
  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);

  species second_species(1);
  species third_species(1);


  second_species.add_indiv(father);
  third_species.add_indiv(uncle);

  species fourth_species(2);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);


  fourth_species.add_indiv(son);
  fourth_species.add_indiv(nephew);

  spp.push_back(first_species);
  spp.push_back(second_species);
  spp.push_back(third_species);
  spp.push_back(fourth_species);

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_5() noexcept
{
  /*
   [3]
    |
    |
   [1]  [2]
    |  /
    | /
   [0]
  */

  const auto p = create_article_parameters();
  //Create individuals
  const indiv grandfather;
  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  const indiv son = create_offspring(father, father, p);
  //Creates species
  const species first_species(0, { grandfather, grandfather } );
  const species second_species(1, { father, father } );
  const species third_species(1, { uncle } );
  const species fourth_species(2, { son } );

  return create_graph_from_species_vector( { first_species, second_species, third_species, fourth_species} );
}

sado::species_graph sado::create_test_graph_6() noexcept
{
  sado::parameters p = create_article_parameters();
  const species first_species(0, { indiv() } );
  const species second_species(1, { create_offspring(first_species[0], first_species[0], p)});
  return create_graph_from_species_vector( { first_species, second_species} );
}

sado::species_graph sado::create_test_graph_7() noexcept
{
  const species first_species(0, { indiv() } );
  return create_graph_from_species_vector( { first_species } );
}
sado::species_graph sado::create_test_graph_8() noexcept
{
  /*
    [4]         [4]
   / | \         |
  /  |  \        |
 [1][2][3] ->   [1]
  \  |  /        |
   \ | /         |
    [0]         [0]
  */


  const auto p = create_article_parameters();

  const indiv grandfather;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  const indiv aunt = create_offspring(grandfather,grandfather,p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);
  const indiv niece = create_offspring(aunt, aunt, p);

  const species first_species(0, { grandfather });
  const species second_species(1, { father } );
  const species third_species(1, { uncle }  );
  const species fourth_species(1, { aunt }  );
  const species fifth_species(2, { son, nephew, niece } );

  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_9() noexcept
{
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


  const auto p = create_article_parameters();

  const indiv grandfather;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);


  const indiv grandson = create_offspring(son, son, p);
  const indiv granddaughter = create_offspring(nephew, nephew,p);

  const species first_species(0, { grandfather });
  const species second_species(1, { father } );
  const species third_species(1, { uncle }  );
  const species fourth_species(2, { son }  );
  const species fifth_species(2, { nephew } );
  const species sixth_species(3, { grandson, granddaughter } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species,
    sixth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_10() noexcept
{

/*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  |  / |       |
  | /  |       |
 [0]  [1]     [0]
  */


  const auto p = create_article_parameters();

  const indiv grandfather;
  const indiv grandmother;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  const indiv aunt = create_offspring(grandmother, grandmother, p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);


  const species first_species(0, { grandfather });
  const species second_species(0, { grandmother } );
  const species third_species(1, { father }  );
  const species fourth_species(1, { uncle, aunt }  );
  const species fifth_species(2, { son, nephew } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_11() noexcept
{

  /*

 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  | \  |       |
  |  \ |       |
 [0]  [1]     [0]

  */


  const auto p = create_article_parameters();

  const indiv grandfather;
  const indiv grandmother;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  const indiv aunt = create_offspring(grandmother, grandmother, p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);


  const species first_species(0, { grandfather });
  const species second_species(0, { grandmother } );
  const species third_species(1, { father, uncle }  );
  const species fourth_species(1, { aunt }  );
  const species fifth_species(2, { son, nephew } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_12() noexcept
{

/*
     [5]         [5]
    / | \         |
   /  |  \        |
 [2] [3] [4] ->  [2]
    \ | / |       |
     \|/  |       |
     [0] [1]     [0]
  */


  const auto p = create_article_parameters();

  const indiv grandfather;
  const indiv grandmother;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  const indiv uncle2 = create_offspring(grandfather,grandfather,p);
  const indiv aunt = create_offspring(grandmother, grandmother, p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);
  const indiv niece = create_offspring(aunt, aunt, p);


  const species first_species(0, { grandfather });
  const species second_species(0, { grandmother } );
  const species third_species(1, { father }  );
  const species fourth_species(1, { uncle }  );
  const species fifth_species(1, { aunt, uncle2 } );
  const species sixth_species(2, { son, nephew, niece} );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species,
    sixth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_13() noexcept
{

  /*

     [3] [4]     [3] [4]
      | / |       | /
      |/  |       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]
  */


  const auto p = create_article_parameters();

  const indiv grandfather;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);
  const indiv brother = create_offspring(father, father, p);

  const species first_species(0, { grandfather });
  const species second_species(1, { father } );
  const species third_species(1, { uncle }  );
  const species fourth_species(2, { son }  );
  const species fifth_species(2, { brother, nephew } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_14() noexcept
{

  /*

     [3] [4]     [3] [4]
      |\  |       | /
      | \ |       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]
  */


  const auto p = create_article_parameters();

  const indiv grandfather;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);
  const indiv brother = create_offspring(uncle, uncle, p);

  const species first_species(0, { grandfather });
  const species second_species(1, { father } );
  const species third_species(1, { uncle }  );
  const species fourth_species(2, { son, brother }  );
  const species fifth_species(2, { nephew } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_15() noexcept
{

  /*
                           time

     [3]            [3]      + present, generation 2
      |              |       |
      |              |       |
     [2]      ->    [2]      + past, generation 1
      | \            |       |
      |  \           |       |
     [0] [1]        [0]      + past, generation 0

  */


  const auto p = create_article_parameters();

  const indiv grandfather;
  const indiv grandmother;

  const indiv father = create_offspring(grandfather, grandmother,p);
  const indiv son = create_offspring(father, father, p);

  const species s0(0, { grandfather });
  const species s1(0, { grandmother });
  const species s2(1, { father } );
  const species s3(2, { son  }  );
  const std::vector<species> spp = { s0, s1, s2, s3 };
  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_16() noexcept
{

  /*

     [3] [4]
      |   |
      |   |
     [1] [2]
      | /
      |/
     [0]
  */


  const auto p = create_article_parameters();

  const indiv grandfather;

  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);

  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);

  const species first_species(0, { grandfather });
  const species second_species(1, { father } );
  const species third_species(1, { uncle }  );
  const species fourth_species(2, { son }  );
  const species fifth_species(2, { nephew } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_17() noexcept
{
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


  const auto p = create_article_parameters();

  //gen 0
  const indiv grandfather;
  //gen 1
  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  //gen 2
  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);
  const indiv niece = create_offspring(uncle, uncle, p);
  //gen 3
  const indiv grandson = create_offspring(son, son, p);
  const indiv granddaughter = create_offspring(nephew, nephew, p);
  const indiv grandniece = create_offspring(niece, niece, p);
  //gen 4
  const indiv grandgrandson = create_offspring(grandson, grandson, p);
  const indiv grandgranddaughter = create_offspring(grandniece, grandniece, p);

  const species first_species(0, { grandfather });
  const species second_species(1, { father } );
  const species third_species(1, { uncle }  );
  const species fourth_species(2, { son }  );
  const species fifth_species(2, { nephew } );
  const species sixth_species(2, { niece } );
  const species seventh_species(3, { grandson, granddaughter } );
  const species eighth_species(3, { grandniece } );
  const species nineth_species(4, { grandgrandson, grandgranddaughter } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species,
    fifth_species,
    sixth_species,
    seventh_species,
    eighth_species,
    nineth_species
  };

  return create_graph_from_species_vector(spp);
}

int sado::count_n_generations(const sado::species_graph& g)
{
  const std::vector<species> spp = get_species_vertexes(g);
  assert(!spp.empty());

  return (*std::max_element(
    std::begin(spp),
    std::end(spp),
    [](const species& lhs, const species& rhs)
    {
      return lhs.get_generation() < rhs.get_generation();
    }
  )).get_generation() + 1;
}


int sado::count_number_species_in_generation(const sado::species_graph& g, const int gen)
{
  if (!(gen <= count_n_generations(g)))
    throw std::invalid_argument("Too high generation");

  const auto vs = vertices(g);

  return std::count_if(
    vs.first,
    vs.second,
    [g, gen](const auto vd)
    {
      return g[vd].get_generation() == gen;
    }
  );
}

std::vector<sado::species> sado::get_descendants(const sp_vert_desc vd, const species_graph& g)
{
  std::vector<sp_vert_desc> v = get_next_generation_vds(vd, g);

  while (1)
  {
    const auto sz_before = v.size();
    //Get the vertex descriptors before all the current vertex descriptors
    v = get_next_generation_vds(v, g);
    const auto sz_after = v.size();
    assert(sz_after >= sz_before);
    if (sz_before == sz_after) break;
  }

  std::vector<species> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [g](const auto vd_next_gen)
    {
      return g[vd_next_gen];
    }
  );
  return w;
}

int sado::get_last_descendant_generation(const sp_vert_desc vd, const species_graph& g)
{
  const std::vector<species> descendants = get_descendants(vd, g);

  //If there are no descendants, this individual *is* its last descendant
  if (descendants.empty())
  {
    return g[vd].get_generation();
  }

  assert(!descendants.empty());
  return (*std::max_element(
    std::begin(descendants),
    std::end(descendants),
    [](const species& lhs, const species& rhs)
    {
      return lhs.get_generation() < rhs.get_generation();
    }
  )).get_generation();
}

std::vector<sado::sp_vert_desc> sado::get_next_generation_vds(sp_vert_desc vd, const species_graph& g)
{
  std::vector<sp_vert_desc> v;

  //Collect the adjacent vds
  const auto vip = boost::adjacent_vertices(vd, g);
  std::copy(
    vip.first,
    vip.second,
    std::back_inserter(v)
  );

  //Keep those of the next/younger generation
  //Thus remove older
  const auto new_end = std::remove_if(
    std::begin(v),
    std::end(v),
    [vd, g](const sp_vert_desc vd_adj)
    {
      //Remove if older
      return g[vd_adj].get_generation() < g[vd].get_generation();
    }
  );
  v.erase(new_end, std::end(v));
  return v;

}

std::vector<sado::sp_vert_desc> sado::get_next_generation_vds(
    const std::vector<sp_vert_desc>& vds, const species_graph& g)
{
  //Keep the old
  std::vector<sp_vert_desc> v = vds;
  for (const auto vd: vds)
  {
    const std::vector<sp_vert_desc> w = get_next_generation_vds(vd, g);
    std::copy(begin(w), std::end(w), std::back_inserter(v));
  }
  //Remove duplicates
  std::sort(std::begin(v), std::end(v));
  const auto new_end = std::unique(std::begin(v), std::end(v));
  v.erase(new_end, std::end(v));


  return v;
}

std::vector<sado::species> sado::get_related(const sp_vert_desc vd, const species_graph& g)
{
  std::vector<species> v;
  v.reserve(boost::degree(vd, g));

  const auto vip = boost::adjacent_vertices(vd, g);
  std::transform(
    vip.first,
    vip.second,
    std::back_inserter(v),
    [g](const sp_vert_desc vd_other)
    {
      return g[vd_other];
    }
  );
  return v;
}

bool sado::has_ancestor(const sp_vert_desc vd, const species_graph& g)
{
  const int focal_generation = g[vd].get_generation();
  const auto related = get_related(vd, g);
  return std::count_if(
    std::begin(related)      ,
    std::end(related),
    [focal_generation](const species& relative)
    {
      return relative.get_generation() < focal_generation;
    }
  );
}

bool sado::has_common_descendant(const sp_vert_desc vd_a, const sp_vert_desc vd_b, const species_graph& g)
{
  assert(g[vd_a].get_generation() == g[vd_b].get_generation());
  assert(vd_a != vd_b);
  std::vector<sp_vert_desc> a = get_next_generation_vds(vd_a, g);
  std::vector<sp_vert_desc> b = get_next_generation_vds(vd_b, g);
  while (1)
  {
    //If there is overlap between a and b, they share a common descendent
    if (has_intersection(a,b)) return true;

    //If not, get the next generation of a and b
    const std::vector<sp_vert_desc> a_new = get_next_generation_vds(a, g);
    const std::vector<sp_vert_desc> b_new = get_next_generation_vds(b, g);
    if (a == a_new && b == b_new) return false;

    //Do the next generation
    a = a_new;
    b = b_new;
  }
}

bool sado::has_extant_descendant(const sp_vert_desc vd, const species_graph& g)
{
  return count_n_generations(g) == get_last_descendant_generation(vd, g) + 1;
}

bool sado::has_intersection(std::vector<sp_vert_desc> a, std::vector<sp_vert_desc> b) noexcept
{
  std::sort(std::begin(a), std::end(a));
  std::sort(std::begin(b), std::end(b));
  std::vector<sp_vert_desc> v_intersection;
  std::set_intersection(
    std::begin(a), std::end(a),
    std::begin(b), std::end(b),
    std::back_inserter(v_intersection)
  );
  return !v_intersection.empty();
}

void sado::remove_cleared_vertices(species_graph& g) noexcept
{
  while (1)
  {
    bool done = true;
    const auto vip = vertices(g);
    for (auto vi = vip.first; vi != vip.second; ++vi)
    {
      if (boost::degree(*vi, g) == 0)
      {
        boost::remove_vertex(*vi, g);
        done = false;
        break;
      }
    }
    if (done) return;
  }
}

void sado::save_to_png(const species_graph& g, const std::string& filename)
{
  {
    std::ofstream f("save_to_png.dot");
    f << g;
  }
  convert_dot_to_svg("save_to_png.dot", "save_to_png.svg");
  convert_svg_to_png("save_to_png.svg", filename);
}

std::string sado::to_newick(const species_graph& /* g */)
{
  //std::string n{"(A,B)"};
  return "";
}

void sado::transfer_connections(
  const sp_vert_desc source,
  const sp_vert_desc target,
  species_graph& g
)
{
  //Get the vertices 'source' is connected to, and connect those to 'target'
  const auto vip = boost::adjacent_vertices(source, g);
  for (auto vi = vip.first; vi != vip.second; ++vi)
  {
    if(!has_edge_between_vertices(*vi, target,g))
    {
    boost::add_edge(*vi, target, g);
    }
  }

  //Time to let go
  boost::clear_vertex(source, g);
}

std::ostream& sado::operator<<(std::ostream& os, const species_graph& g) noexcept
{
  boost::write_graphviz(os, g,
    species_graph_vertex_writer<species_graph>(g)
  );
  return os;
}

