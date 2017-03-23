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
  const auto g = create_ancestry_graph(r.get_species());
  const auto n = to_newick(g);
  BOOST_CHECK(!n.empty());
  //const auto h = create_reconstructed(g);
  //const auto newick_reconstructed = to_newick(h);

}
