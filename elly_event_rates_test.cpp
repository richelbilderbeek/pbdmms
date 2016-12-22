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
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  {
    const double anagenesis_rate{0.25};
    const int n_global_species{80};
    const double expected{20.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species)};
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_cladogenesis_main)
{
  {
    const double clad_rate_main{0.02};
    const int carrying_cap_main{500};
    const int n_mainland_only{200};
    const int n_mainland{350};
    const double expected{1.2};
    const double measured{calc_clad_mainland(clad_rate_main,
                                             n_mainland_only,
                                             n_mainland,
                                             carrying_cap_main)};
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  {
   const double clad_rate_main{0.5};
   const int carrying_cap_main{750};
   const int n_mainland_only{0};
   const int n_mainland{0};
   const double expected{0.0};
   const double measured{calc_clad_mainland(clad_rate_main,
                                            n_mainland,
                                            n_mainland_only,
                                            carrying_cap_main)};
   BOOST_CHECK_CLOSE(expected, measured, 0.0001);
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
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
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
   BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_clad_main)
{
  {
    const double clad_rate_main{0.01};
    const int n_both{10};
    const int n_main{250};
    const int carrying_cap_main{500};
    const double expected{0.05};
    const double measured{calc_glob_clad_mainland(
      clad_rate_main,
      n_both,
      n_main,
      carrying_cap_main)};
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  {
    const double clad_rate_main{0.1};
    const int n_both{0};
    const int n_main{250};
    const int carrying_cap_main{500};
    const double expected{0.0};
    const double measured{
      calc_glob_clad_mainland(
        clad_rate_main,
        n_both,
        n_main,
        carrying_cap_main
      )
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_iclad)
{
  {
    const double rate_clad_is{0.02};
    const int n_island_only{30};
    const int n_species_within_clade_d{30};
    const int carrying_cap_is{60};
    const double expected{0.3};
    const double measured{
      calc_iclad(
        rate_clad_is,
        n_island_only,
        n_species_within_clade_d,
        carrying_cap_is
      )
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
  {
    const double rate_clad_is{0.02};
    const int n_island_only{0};
    const int n_species_within_clade_d{0};
    const int carrying_cap_is{60};
    const double expected{0.0};
    const double measured{calc_iclad(rate_clad_is,
                                     n_island_only,
                                     n_species_within_clade_d,
                                     carrying_cap_is)};
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_islands_ext_rate_on_island)
{
  {
    const double ext_rate_is{0.01};
    const int n_island_only{40};
    const double expected{0.4};
    const double measured{calc_islands_ext_rate_on_island(
            ext_rate_is,
            n_island_only)};
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_mainlands_ext_rate_on_mainland)
{
  {
    const double ext_rate_main{0.01};
    const int n_mainland_only{400};
    const double expected{4.0};
    const double measured{calc_mainlands_ext_rate_on_mainland(
            ext_rate_main,
            n_mainland_only)};
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_migration_to_island)
{
  {
    const double mig_rate_main{0.02};
    const int n_species_in_clade_d{5};
    const int carrying_cap_is{30};
    const int n_mainland_species{300};
    const double expected{5.0};
    const double measured{calc_migration_to_island(
            mig_rate_main,
            n_species_in_clade_d,
            carrying_cap_is,
            n_mainland_species)};
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_spec_ext_rate_on_mainland)
{
  {
    const double ext_rate_main{0.2};
    const int n_both{20};
    const double expected{4.0};
    const double measured{calc_glob_spec_ext_rate_on_mainland(
            ext_rate_main,
            n_both)};
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_spec_ext_rate_on_island)
{
  {
    const double ext_rate_is{0.2};
    const int n_both{20};
    const double expected{4.0};
    const double measured{calc_glob_spec_ext_rate_on_island(
            ext_rate_is,
            n_both)};
    BOOST_CHECK_CLOSE(expected, measured, 0.001);

  }
}

#pragma GCC diagnostic pop
