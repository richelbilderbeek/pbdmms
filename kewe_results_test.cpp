#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <cassert>
#include <vector>

#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_individual.h"
#include "kewe_ses.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace kewe;






BOOST_AUTO_TEST_CASE(kewe_results_test_count_1_species)
{
  const parameters p_a;
  std::mt19937 gen(42);
  individual a(p_a);
  a.init(p_a,gen);

  individuals pop(4,a);

  int n_of_species{count_good_species(pop, p_a)};
  BOOST_CHECK_EQUAL(n_of_species, 1);
}

BOOST_AUTO_TEST_CASE(kewe_results_test_count_2_species)
{
  const parameters p_a;
  std::mt19937 gen(42);
  individual a(p_a);
  a.init(p_a,gen);

  kewe::parameters p_b;
  p_b.m_sim_parameters.p0 = -0.5;
  p_b.m_sim_parameters.q0 = -0.5;

  individual b(p_b);
  b.init(p_b,gen);

  individuals pop(4,a);
  pop.push_back(b);
  pop.push_back(b);

  int n_of_species{count_good_species(pop, p_a)};
  BOOST_CHECK_EQUAL(n_of_species, 2);
}

BOOST_AUTO_TEST_CASE(kewe_results_test_count_1_species_again)
{
  const parameters p_a;
  std::mt19937 gen(42);
  individual a(p_a);
  a.init(p_a,gen);

  parameters p_b;
  p_b.m_sim_parameters.p0 = -0.5;

  individual b(p_b);
  b.init(p_b,gen);

  individuals pop(4,a);
  pop.push_back(b);
  pop.push_back(b);

  int n_of_species{count_good_species(pop, p_a)};
  BOOST_CHECK_EQUAL(n_of_species, 1);
}

BOOST_AUTO_TEST_CASE(kewe_results_test_count_species_through_time)
{
  const parameters p_a;
  std::mt19937 gen(42);
  individual a(p_a);
  a.init(p_a,gen);



  individuals pop(4,a);
  std::vector<std::pair<bigint,int>> ltt_plot;
  output_ltt(pop, 10, p_a, ltt_plot);

  parameters p_b;
  p_b.m_sim_parameters.p0 = -0.5;
  p_b.m_sim_parameters.q0 = -0.5;

  individual b(p_b);
  b.init(p_b,gen);

  pop.push_back(b);
  pop.push_back(b);

  output_ltt(pop, 20, p_a, ltt_plot);

  BOOST_CHECK(ltt_plot[0].first < ltt_plot[1].first);
  BOOST_CHECK(ltt_plot[0].second < ltt_plot[1].second);
}


///Old tests for "hack"
/*
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
  kewe::parameters parameters;
  BOOST_CHECK_THROW(outputLTT(w, w, empty, parameters), std::invalid_argument);
  BOOST_CHECK_THROW(outputLTT(w, empty, w, parameters), std::invalid_argument);
  BOOST_CHECK_THROW(outputLTT(empty, w, w, parameters), std::invalid_argument);

}
*/

#pragma GCC diagnostic pop



