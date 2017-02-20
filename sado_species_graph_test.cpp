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

BOOST_AUTO_TEST_CASE(sado_create_test_graph_1)
{
  const auto g = create_test_graph_1();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 8);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 11);
  save_graph_to_dot(g, "test_graph_1.dot");
  convert_dot_to_svg("test_graph_1.dot", "test_graph_1.svg");
  convert_svg_to_png("test_graph_1.svg","test_graph_1.png");
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_graph_from_species_graph)
{
  {
    const auto g = create_test_graph_2();
    const auto r = create_reconstructed_graph_from_species_graph(g);
    //BOOST_CHECK_EQUAL(g, r);
  }
}


#ifdef sado_has_extant_descendant_WORKS
//TODO
BOOST_AUTO_TEST_CASE(sado_count_number_reconstructed_species_in_generation)
{
  const auto g = create_test_graph_1();
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 2), 2);
}
#endif // sado_has_extant_descendant_WORKS

#pragma GCC diagnostic pop
