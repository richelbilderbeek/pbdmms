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

BOOST_AUTO_TEST_CASE(sado_issue_264)
{
  simulation s(create_issue_264_parameters());
  s.run();
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r);
  const auto n = to_newick(g);
  BOOST_CHECK(!n.empty());
  const auto h = create_reconstructed(g);
  const auto newick_reconstructed = to_newick(h);
}

BOOST_AUTO_TEST_CASE(sado_unrun_simulation_has_a_phylogeny_of_one_node)
{
  simulation s(create_issue_264_parameters());
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r);
  BOOST_CHECK_EQUAL(1, boost::num_vertices(g));
}

BOOST_AUTO_TEST_CASE(sado_unrun_simulation_has_a_dull_newick)
{
  const simulation s(create_article_parameters());
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r);
  const auto n = to_newick(g);
  BOOST_CHECK_EQUAL(n, "(:0);");
  const auto h = create_reconstructed(g);
  const auto newick_reconstructed = to_newick(h);
  BOOST_CHECK_EQUAL(newick_reconstructed, "(:0);");
}

BOOST_AUTO_TEST_CASE(sado_simulation_one_timestep_has_a_phylogeny_of_two_nodes)
{
  simulation s(create_issue_264_parameters());
  s.do_timestep();
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r);
  BOOST_CHECK_EQUAL(2, boost::num_vertices(g));
}

BOOST_AUTO_TEST_CASE(sado_simulation_one_timestep_has_a_simple_newick)
{
  simulation s(create_article_parameters());
  s.do_timestep();
  const results& r = s.get_results();
  const auto g = create_ancestry_graph(r);
  const auto n = to_newick(g);
  BOOST_CHECK_EQUAL(n, "(:1);");
  const auto h = create_reconstructed(g);
  const auto newick_reconstructed = to_newick(h);
  BOOST_CHECK_EQUAL(newick_reconstructed, "(:1);");
}


