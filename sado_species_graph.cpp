#include "sado_species_graph.h"

#include <boost/graph/adjacency_list.hpp>
#include  "count_undirected_graph_connected_components.h"
//#include <boost/range/algorithm.hpp>

#include "has_edge_between_vertices.h"
#include "get_vertex_out_degrees.h"
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
  const int n_generations{count_n_generations(g)};
  const auto vs = vertices(g);
  for (auto vd = vs.first; vd != vs.second; ++vd)
  {
    if (!has_extant_descendant(*vd, g, n_generations))
    {
      boost::clear_vertex(*vd, g);
    }
  }
}

std::vector<sado::sp_vert_desc> sado::collect_root_vds(const species_graph& g)
{
  std::vector<sp_vert_desc> vds;
  const auto vip = boost::vertices(g);
  std::copy_if(
    vip.first,
    vip.second,
    std::back_inserter(vds),
    [g](const auto vd)
    {
      return g[vd].get_generation() == 0;
    }
  );
  return vds;
}

std::vector<sado::sp_vert_desc> sado::collect_younger_nodes(
  const sp_vert_desc vd,
  const species_graph& g)
{
  assert(!is_tip(vd, g));

  //First get all younger vds
  std::vector<sp_vert_desc> vds{get_next_generation_vds(vd, g)};

  //For each of those younger vertex descriptors (yvd),
  //walk towards the present until there is a node
  for (auto& yvd: vds)
  {
    //Even Younger Vertex Descriptor
    while (1)
    {
      const auto eyvds = get_next_generation_vds(yvd, g);
      if (eyvds.empty()) break; //A tip
      if (eyvds.size() > 1) break;
      yvd = eyvds[0];
    }
  }
  return vds;
}

sado::species_graph
sado::create_empty_directed_species_graph() noexcept
{
  return {};
}

sado::species_graph
sado::create_my_species_graph() noexcept
{
  return create_empty_directed_species_graph();
}

sado::species_graph sado::create_graph_from_species_vector(
  const std::vector<species>& s) noexcept
{
  auto g = create_empty_directed_species_graph();

  //Add all species to the graph, and collect the vertex descriptors
  std::vector<sp_vert_desc> v;
  v.reserve(boost::num_vertices(g));
  std::transform(
    std::begin(s),
    std::end(s),
    std::back_inserter(v),
    [&g](const species& t)
    {
      return add_species_vertex(t, g);
    }
  );

  //Try out all combinations of species ...
  for (const sp_vert_desc vd_kid: v)
  {
    for (const sp_vert_desc vd_ancestor: v)
    {
      //No self-loops
      if (vd_ancestor == vd_kid) continue;

      //Don't bother if they are already connected
      if (has_edge_between_vertices(vd_kid, vd_ancestor, g)) continue;

      const species& kid = g[vd_kid];
      const species& ancestor = g[vd_ancestor];

      //Kids can only be present in species of same and older generation
      const int generations{kid.get_generation() - ancestor.get_generation()};
      if (generations < 0) continue;

      //Draw an edge if a kid is in 'kid' and a father or mother is in 'ancestors'
      if (has_ancestor_and_kid(ancestor, kid))
      {
        //Due to overlapping generations, this kid may be of the
        //same generation as its father or mother
        assert(generations >= 0);
        add_int_edge(vd_kid, vd_ancestor, generations, g);
      }
    }
  }
  return g;
}

sado::species_graph sado::create_reconstructed(species_graph g) noexcept
{
  if (boost::num_vertices(g) <= 1)
  {
    return g;
  }

  //Remove the edges that span more generations
  //std::cerr << "Remove the edges that span more generations\n";
  remove_multi_generation_edges(g);

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

  return g;
}

sado::species_graph sado::create_test_graph_1() noexcept
{
  const auto p = create_article_parameters();

  const indiv ia;
  const indiv ib;
  const indiv ic;
  const indiv id;

  const indiv kid1 = create_offspring(ia,ib,p);
  const indiv kid2 = create_offspring(ia,ic,p);
  const indiv kid3 = create_offspring(ib,ic,p);
  const indiv kid4 = create_offspring(ia,ib,p);
  const indiv kid5 = create_offspring(ic,id,p);

  const indiv kidkid1 = create_offspring(kid1, kid2, p);
  const indiv kidkid2 = create_offspring(kid3,kid4,p);

  const species sa(0, { ia, ib});
  const species sb(0, { ic, id} );
  const species sc(1, { kid1} );
  const species sd(1, { kid2} );
  const species se(1, { kid3} );
  const species sf(1, { kid4, kid5 } );
  const species sg(2, { kidkid1 });
  const species sh(2, { kidkid2 } );

  const std::vector<species> spp = {
    sa, sb, sc, sd, se, sf, sg, sh
  };

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
  const indiv i;
  const indiv j;

  const auto p = create_article_parameters();
  const indiv kid1 = create_offspring(i,j,p);
  const indiv kid2 = create_offspring(i,j,p);
  const indiv kidkid1 = create_offspring(kid1, kid2, p);
  const indiv kidkid2 = create_offspring(kid1, kid2, p);

  const species sa(0, {i, j});
  const species sb(1, {kid1, kid2});
  const species sc(2, {kidkid1} );
  const species sd(2, {kidkid2});
  const std::vector<species> spp = { sa, sb, sc, sd };
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

  const auto p = create_article_parameters();

  const indiv grandfather;
  const indiv father = create_offspring(grandfather,grandfather,p);
  const indiv uncle = create_offspring(grandfather,grandfather,p);
  const indiv son = create_offspring(father, father, p);
  const indiv nephew = create_offspring(uncle, uncle, p);

  const species sa(0, {grandfather});
  const species sb(1, {father});
  const species sc(1, {uncle});
  const species sd(2, {son,nephew});
  const std::vector<species> spp = { sa, sb, sc, sd };

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
  const species sa(0, { grandfather  } );
  const species sb(1, { father } );
  const species sc(1, { uncle } );
  const species sd(2, { son } );

  return create_graph_from_species_vector( { sa, sb, sc, sd} );
}

sado::species_graph sado::create_test_graph_6() noexcept
{
  /*
   [1]
    |
    |
   [0]
  */
  sado::parameters p = create_article_parameters();
  const indiv i;
  const species sa(0, { i } );
  const species sb(1, { create_offspring(i, i, p)});
  return create_graph_from_species_vector( { sa, sb} );
}

sado::species_graph sado::create_test_graph_7() noexcept
{
/*

 [0]

*/
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

  const species sa(0, { grandfather });
  const species sb(1, { father } );
  const species sc(1, { uncle }  );
  const species sd(2, { son }  );
  const species se(2, { nephew } );
  const species sf(2, { niece } );
  const species sg(3, { grandson, granddaughter } );
  const species sh(3, { grandniece } );
  const species si(4, { grandgrandson, grandgranddaughter } );
  const std::vector<species> spp = {
    sa, sb, sc, sd, se, sf, sg, sh, si
  };

  return create_graph_from_species_vector(spp);
}



sado::species_graph sado::create_test_graph_18() noexcept
{
  /*

    1 +  {b} {c}
      |   |  /
      |   | /
    0 +  {a}

    t (generation)

  */

  const auto p = create_article_parameters();
  //Create individuals
  const indiv a;
  const indiv b = create_offspring(a,a,p);
  const indiv c = create_offspring(a,a,p);
  //Creates species
  const species s_a(0, { a } );
  const species s_b(1, { b } );
  const species s_c(1, { c } );

  return create_graph_from_species_vector( { s_a, s_b, s_c } );
}

sado::species_graph sado::create_test_graph_19() noexcept
{
  /*

   2 +    {d}   {e}   {f}
     |      \     \   /
     |       \     \ /
   1 +       {b}   {c}
     |         \   /
     |          \ /
   0 +          {a}

     t (generation)
  */

  const auto p = create_article_parameters();
  //Create individuals
  const indiv a;
  const indiv b = create_offspring(a,a,p);
  const indiv c = create_offspring(a,a,p);
  const indiv d = create_offspring(b,b,p);
  const indiv e = create_offspring(c,c,p);
  const indiv f = create_offspring(c,c,p);
  //Creates species
  const species s_a(0, { a } );
  const species s_b(1, { b } );
  const species s_c(1, { c } );
  const species s_d(2, { d } );
  const species s_e(2, { e } );
  const species s_f(2, { f } );

  return create_graph_from_species_vector(
    { s_a, s_b, s_c, s_d, s_e, s_f }
  );
}

sado::species_graph sado::create_test_graph_20() noexcept
{
  /*

      [3]         [2]
     / | \         |
    /  |  \        |
   |  [1]--[2] -> [1]
    \  |  /        |
     \ | /         |
      [0]         [0]

   */
  const auto p = create_article_parameters();

  //gen 0
  const indiv grandfather, grandmother; // both species 0, grandmother species 2
  //gen 1
  const indiv father = create_offspring(grandfather, grandmother,p); // species 1
  const indiv mother = create_offspring(grandfather, grandmother, p); // species 2
    //gen 2
  const indiv son = create_offspring(father, father, p); // species 3
  const indiv daughter = create_offspring(grandmother, grandmother, p); //species 3


  const species first_species(0, { grandfather, grandmother });
  const species second_species(1, { father } );
  const species third_species(1, { grandmother, mother }  );
  const species fourth_species(2, { son, daughter } );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
    fourth_species
  };

  return create_graph_from_species_vector(spp);
}

sado::species_graph sado::create_test_graph_21() noexcept
{
  /*

      [2]        [2]
     / |          |
    |  |          |
    | [1]   -->  [1]
    \  |          |
     \ |          |
      [0]        [0]
   */


  const auto p = create_article_parameters();

  //gen 0
  const indiv grandfather, grandmother;
  //gen 1
  const indiv father = create_offspring(grandfather,grandfather,p);
  //gen 2
  const indiv son = create_offspring(father, father, p);
  const indiv daughter = create_offspring(grandmother, grandmother, p);


  const species first_species(0, { grandfather, grandmother });
  const species second_species(1, { father, grandmother } );
  const species third_species(2, { son, daughter }  );
  const std::vector<species> spp =
  {
    first_species,
    second_species,
    third_species,
  };

  return create_graph_from_species_vector(spp);
}

std::vector<sado::species_graph> sado::create_test_graphs() noexcept
{
  return
  {
    create_test_graph_1(),
    create_test_graph_2(),
    create_test_graph_3(),
    create_test_graph_4(),
    create_test_graph_5(),
    create_test_graph_6(),
    create_test_graph_7(),
    create_test_graph_8(),
    create_test_graph_9(),
    create_test_graph_10(),
    create_test_graph_11(),
    create_test_graph_12(),
    create_test_graph_13(),
    create_test_graph_14(),
    create_test_graph_15(),
    create_test_graph_16(),
    create_test_graph_17(),
    create_test_graph_18(),
    create_test_graph_19()
  };
}

int sado::count_n_generations(const species_graph& g)
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

int sado::count_number_species_in_generation(const species_graph& g, const int gen)
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

int sado::count_n_extant(const species_graph& g)
{
  assert(boost::num_vertices(g));
  const int t_last_gen{count_n_generations(g) - 1};
  assert(t_last_gen >= 0);
  return count_number_species_in_generation(g, t_last_gen);
}

std::vector<sado::species> sado::get_descendants(
  const sp_vert_desc vd, const species_graph& g)
{
  //std::cerr << "Getting the vds for the next generation\n";
  std::vector<sp_vert_desc> v = get_next_generation_vds(vd, g);

  //Use a depth-first search
  while (1) //#264
  {
    //std::cerr << v.size() << ',';
    const auto sz_before = v.size();
    //Get the vertex descriptors before all the current vertex descriptors
    v = get_next_generation_vds(v, g);
    const auto sz_after = v.size();
    assert(sz_after >= sz_before);
    if (sz_before == sz_after) break;
  }

  //std::cerr << "Converting the vds to species\n";
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
  //std::cerr << "Get the descendants:\n";
  const std::vector<species> descendants = get_descendants(vd, g);

  //std::cerr << "See if there are no descendants:\n";
  //If there are no descendants, this individual *is* its last descendant
  if (descendants.empty())
  {
    return g[vd].get_generation();
  }

  //std::cerr << "find descencendant:\n";
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

std::vector<sado::sp_vert_desc> sado::get_next_generation_vds(
  sp_vert_desc vd, const species_graph& g)
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

bool sado::has_common_descendant(
  const sp_vert_desc vd_a, const sp_vert_desc vd_b, const species_graph& g)
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
  return has_extant_descendant(vd, g, count_n_generations(g));
}

bool sado::has_extant_descendant(const sp_vert_desc vd, const species_graph& g,
  const int n_generations)
{
  return n_generations == get_last_descendant_generation(vd, g) + 1;
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

bool sado::is_tip(const sp_vert_desc vd, const species_graph& g)
{
  return g[vd].get_generation() == count_n_generations(g) - 1;
}

bool sado::may_transfer(sp_vert_desc from, sp_vert_desc to, const species_graph& g)
{
  //Cannot move to self
  if (from == to) return false;
  //Cannot move between generations
  if (g[from].get_generation() != g[to].get_generation()) return false;
  //Must share a descent to merge
  if (!has_common_descendant(from, to, g)) return false;
  return true;
}

void sado::merge_split_species(species_graph& g)
{
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
      if (!may_transfer(*vi_lagging, *vi_leading, g)) continue;

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

void sado::remove_multi_generation_edges(species_graph& g)
{
  boost::remove_edge_if(
    [&g](const auto ed)
    {
      const auto vd_from = boost::source(ed, g);
      const auto vd_to = boost::target(ed, g);
      const auto t_from = g[vd_from].get_generation();
      const auto t_to = g[vd_to].get_generation();
      const auto dt = std::abs(t_to - t_from);
      return dt > 1;
    },
    g
  );
}

void sado::remove_self_loops(species_graph& g)
{
  boost::remove_edge_if(
    [&g](const auto ed)
    {
      return boost::source(ed, g) == boost::target(ed, g);
    },
    g
  );
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

