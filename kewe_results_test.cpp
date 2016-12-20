#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <cassert>
#include <vector>

#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_individual.h"
#include "kewe_SES.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(kewe_results_test_calculate_attractiveness)
{
 const kewe_parameters p_a;
 kewe_parameters p_b;
 std::mt19937 gen(42);
 
 p_b.sim_parameters.x0 = -0.5;
 p_b.sim_parameters.p0 = -0.5;
 p_b.sim_parameters.q0 = -0.5;
 
 const indiv a(p_a);
 indiv b(p_a);
 b.init(p_b, gen);
 
 BOOST_CHECK(a != b);

 std::vector<indiv> pop;
 
 pop.push_back(a);
 pop.push_back(a);
 pop.push_back(b);
 pop.push_back(b);
 
 std::vector<std::vector<double>> results = calc_attractiveness_indivs(pop, p_a);
 
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

/*BOOST_AUTO_TEST_CASE(kewe_results_test_count_1_species)
{
  const kewe_parameters p_a;
  std::mt19937 gen(42);
  indiv a(p_a);
  a.init(p_a,gen);

  std::vector<indiv> pop(4,a);

  int n_of_species{count_good_species(pop, p_a)};
  BOOST_CHECK_EQUAL(n_of_species, 1);
}

BOOST_AUTO_TEST_CASE(kewe_results_test_count_2_species)
{
  const kewe_parameters p_a;
  std::mt19937 gen(42);
  indiv a(p_a);
  a.init(p_a,gen);

  kewe_parameters p_b;
  p_b.sim_parameters.p0 = -0.5;
  p_b.sim_parameters.q0 = -0.5;

  indiv b(p_b);
  b.init(p_b,gen);

  std::vector<indiv> pop(4,a);
  pop.push_back(b);
  pop.push_back(b);

  int n_of_species{count_good_species(pop, p_a)};
  BOOST_CHECK_EQUAL(n_of_species, 2);
}

BOOST_AUTO_TEST_CASE(kewe_results_test_count_1_species_again)
{
  const kewe_parameters p_a;
  std::mt19937 gen(42);
  indiv a(p_a);
  a.init(p_a,gen);

  kewe_parameters p_b;
  p_b.sim_parameters.p0 = -0.5;

  indiv b(p_b);
  b.init(p_b,gen);

  std::vector<indiv> pop(4,a);
  pop.push_back(b);
  pop.push_back(b);

  int n_of_species{count_good_species(pop, p_a)};
  BOOST_CHECK_EQUAL(n_of_species, 1);
}

BOOST_AUTO_TEST_CASE(kewe_results_test_count_num_border)
{
 int numOfBorders{0};
 count_num_border(0.06, 0.06, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.06, 0.06, 0.04, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.06, 0.04, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.04, 0.06, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.06, 0.04, 0.04, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 1);
 count_num_border(0.04, 0.06, 0.04, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 1);
 count_num_border(0.04, 0.04, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 2);
}

BOOST_AUTO_TEST_CASE(kewe_results_should_throw_if_empty_histogram)
{ const std::vector<double> histogram;
  BOOST_CHECK_THROW(int x = countBorders(histogram), std::invalid_argument);

  std::vector<double> v (3, 1.0);
  std::vector<std::vector<double>> w;
  w.push_back(v);

  std::vector<std::vector<double>> empty;

  BOOST_CHECK_THROW(countLineagesForGen(1, w, w, empty), std::invalid_argument);
  BOOST_CHECK_THROW(countLineagesForGen(1, w, empty, w), std::invalid_argument);
  BOOST_CHECK_THROW(countLineagesForGen(1, empty, w, w), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(kewe_results_should_throw_with_negative_time)
{
  std::vector<double> v (3, 1.0);
  std::vector<std::vector<double>> w;
  w.push_back(v);
  BOOST_CHECK_THROW(countLineagesForGen(-1, w, w, w), std::invalid_argument);

}


BOOST_AUTO_TEST_CASE(kewe_outputLTT_should_throw_if_empty_histogram)
{
  std::vector<double> v (3, 1.0);
  std::vector<std::vector<double>> w;
  w.push_back(v);

  std::vector<std::vector<double>> empty;
  kewe_parameters parameters;
  BOOST_CHECK_THROW(outputLTT(w, w, empty, parameters), std::invalid_argument);
  BOOST_CHECK_THROW(outputLTT(w, empty, w, parameters), std::invalid_argument);
  BOOST_CHECK_THROW(outputLTT(empty, w, w, parameters), std::invalid_argument);

}
*/

#pragma GCC diagnostic pop



