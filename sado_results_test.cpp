#include "sado_results.h"
#include "sado_helper.h"

#include <boost/test/unit_test.hpp>

#include "sado_simulation.h"
#include "sado_ancestry_graph.h"
#include "sado_newick.h"

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_create_header_str)
{
  const auto p = create_golden_standard_parameters();
  const std::string measured{create_header_str(p)};
  const std::string expected{get_golden_output()[0]};
  BOOST_CHECK_EQUAL(measured, expected);
}

#ifdef FIX_ISSUE_264
BOOST_AUTO_TEST_CASE(sado_issue_264)
{
  simulation s(create_issue_264_parameters());
  s.run();
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r.get_species());
  const auto n = to_newick(g);
  BOOST_CHECK(!n.empty());
  //const auto h = create_reconstructed(g);
  //const auto newick_reconstructed = to_newick(h);

}
#endif // FIX_ISSUE_264

//#define FIX_ISSUE_268
#ifdef FIX_ISSUE_268
BOOST_AUTO_TEST_CASE(sado_connect_ancestors)
{
  simulation s(create_article_parameters());
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r.get_species());
  const auto n = to_newick(g);
  BOOST_CHECK_EQUAL(n, "(:0);");
  //const auto h = create_reconstructed(g);
  //const auto newick_reconstructed = to_newick(h);

}
#endif // FIX_ISSUE_264

BOOST_AUTO_TEST_CASE(sado_create_ancestry_graph_of_empty_sim)
{
  simulation s(create_issue_264_parameters());
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r.get_species());
  save_to_png(g, "sado_create_ancestry_graph_of_empty_sim.png");
  BOOST_CHECK_EQUAL(r.get_species().size(), boost::num_vertices(g));
}

BOOST_AUTO_TEST_CASE(sado_create_ancestry_graph_of_sim_with_one_timestep)
{
  simulation s(create_issue_264_parameters());
  s.do_timestep();
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r.get_species());
  save_to_png(g, "sado_create_ancestry_graph_of_sim_with_one_timestep.png");
  BOOST_CHECK_EQUAL(r.get_species().size(), boost::num_vertices(g));
}

