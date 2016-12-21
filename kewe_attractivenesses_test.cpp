#include "kewe_attractivenesses.h"

#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_results_test_calculate_attractiveness)
{
 const parameters p_a;
 parameters p_b;
 std::mt19937 gen(42);

 p_b.m_sim_parameters.x0 = -0.5;
 p_b.m_sim_parameters.p0 = -0.5;
 p_b.m_sim_parameters.q0 = -0.5;

 const individual a(p_a);
 individual b(p_a);
 b.init(p_b, gen);

 BOOST_CHECK(a != b);

 individuals pop;

 pop.push_back(a);
 pop.push_back(a);
 pop.push_back(b);
 pop.push_back(b);

 std::vector<std::vector<double>> results = calc_attractivenesses(pop, p_a);

 /*
 0 should be attracted to 1, but not 2 and 3
 1 should be attracted to 0, but not 2 and 3
 2 should be attracted to 3, but not 0 and 1
 3 should be attracted to 2, but not 0 and 1
 */

 BOOST_CHECK(results[0][1] > results[0][2] && results[0][1] > results[0][2]);
 BOOST_CHECK(results[1][0] > results[1][2] && results[1][0] > results[1][3]);
 BOOST_CHECK(results[2][3] > results[2][0] && results[2][3] > results[2][1]);
 BOOST_CHECK(results[3][2] > results[3][0] && results[3][2] > results[3][1]);

}

#pragma GCC diagnostic pop

