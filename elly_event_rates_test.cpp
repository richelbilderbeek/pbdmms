#include "elly_event_rates.h"

#include <cmath>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_calc_anagenesis)
{
  {
    const double anagenesis_rate{0.5};
    const int n_global_species{20};
    const double expected{10.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species)};
    BOOST_CHECK(std::abs(expected - measured) < 0.0001);
  }
  {
    const double anagenesis_rate{0.25};
    const int n_global_species{80};
    const double expected{20.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species)};
    BOOST_CHECK(std::abs(expected - measured) < 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_cladogenesis_main)
{
  {
    const double clad_rate_main{0.02};
    const int carrying_cap_main{500};
    const int n_mainland_only{350};
    const int n_mainland{20};
    const double expected{0.0049189};
    const double measured{calc_clad_mainland(clad_rate_main,
                                             n_mainland_only,
                                             n_mainland,
                                             carrying_cap_main)};
    BOOST_CHECK(std::abs(expected - measured) < 0.0001);
  }
  {
   const double clad_rate_main{0.5};
   const int carrying_cap_main{750};
   const int n_mainland_only{0};
   const int n_mainland{0};
    BOOST_CHECK_THROW(
      calc_clad_mainland(
        clad_rate_main,
        n_mainland_only,
        n_mainland,
        carrying_cap_main
      ),
      std::invalid_argument
    );
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_clad_is)
{
  {
    const double clad_rate_is{0.2};
    const int n_species_within_clade_d{5};
    const int carrying_cap_is{50};
    const int n_both{13};
    const double expected{2.34};
    const double measured{calc_glob_clad_island(
      clad_rate_is,
      n_species_within_clade_d,
      carrying_cap_is,
      n_both)};
    BOOST_CHECK(std::abs(expected - measured) < 0.0001);
  }
  {
   const double clad_rate_is{0.03};
   const int n_species_within_clade_d{0};
   const int carrying_cap_is{50};
   const int n_both{0};
   const double expected{0.0};
   const double measured{calc_glob_clad_island(
     clad_rate_is,
     n_species_within_clade_d,
     carrying_cap_is,
     n_both)};
   BOOST_CHECK(std::abs(expected - measured) < 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_clad_main)
{
  {
    const double clad_rate_main{0.01};
    const int n_both{10};
    const int n_main{250};
    const int carrying_cap_main{500};
    const double expected{0.0002};
    const double measured{calc_glob_clad_mainland(
      clad_rate_main,
      n_both,
      n_main,
      carrying_cap_main)};
    BOOST_CHECK(std::abs( expected - measured) < 0.0001);
  }
  {
    const double clad_rate_main{0.1};
    const int n_both{0};
    const int n_main{250};
    const int carrying_cap_main{500};
    const double expected{0.0};
    const double measured{calc_glob_clad_mainland(
      clad_rate_main,
      n_both,
      n_main,
      carrying_cap_main)};
    BOOST_CHECK(std::abs( expected - measured) < 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_iclad)
{
  {

  }
}

#pragma GCC diagnostic pop
