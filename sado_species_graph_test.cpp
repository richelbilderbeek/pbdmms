#include "sado_species_graph.h"
#include "sado_species.h"
#include "sado_individual.h"
#include "sado_id.h"
#include "save_graph_to_dot.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "sado_parameters.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_check_if_species_get_correctly_graphed)
{
  std::vector<sado_species> species;

  const indiv i;
  const indiv j;
  const indiv k;
  const indiv l;

  sado_species first_species(1);
  sado_species second_species(1);

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


  sado_species third_species(2);
  sado_species fourth_species(2);
  sado_species fifth_species(2);
  sado_species sixth_species(2);

  third_species.add_indiv(kid1);
  fourth_species.add_indiv(kid2);
  fifth_species.add_indiv(kid3);
  sixth_species.add_indiv(kid4);
  sixth_species.add_indiv(kid5);

  const indiv kidkid1 = create_offspring(kid1, kid2, p);
  const indiv kidkid2 = create_offspring(kid3,kid4,p);

  sado_species seventh_species(3);
  sado_species eighth_species(3);

  seventh_species.add_indiv(kidkid1);
  eighth_species.add_indiv(kidkid2);

  species.push_back(first_species);
  species.push_back(second_species);
  species.push_back(third_species);
  species.push_back(fourth_species);
  species.push_back(fifth_species);
  species.push_back(sixth_species);
  species.push_back(seventh_species);
  species.push_back(eighth_species);

  const auto g = create_graph_from_species_vector(species);

  save_graph_to_dot(g, "testing_graph.dot");
  convert_dot_to_svg("testing_graph.dot", "testing_graph.svg");
  convert_svg_to_png("testing_graph.svg","testing_graph.png");
}




#pragma GCC diagnostic pop
