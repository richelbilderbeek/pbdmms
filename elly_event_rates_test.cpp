#include "elly_event_rates.h"

#include <cmath>

#include "elly_parameters.h"
#include "elly_simulation.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_calc_anagenesis)
{
  {
    const per_species_rate anagenesis_rate{0.5};
    const int n_global_species{20};
    const double expected{10.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species).get()};
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  {
    const per_species_rate anagenesis_rate{0.25};
    const int n_global_species{80};
    const double expected{20.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species).get()};
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_cladogenesis_main)
{
  {
    const per_species_rate clad_rate_main{0.02};
    const carrying_capacity carrying_cap_main{500};
    const int n_mainland_only{200};
    const int n_mainland{350};
    const double expected{1.2};
    const double measured{
      calc_clad_mainland(
        clad_rate_main,
        n_mainland,
        n_mainland_only,
        carrying_cap_main
      ).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  {
    const per_species_rate clad_rate_main{0.5};
    const carrying_capacity carrying_cap_main{750};
    const int n_mainland_only{0};
    const int n_mainland{0};
    const double expected{0.0};
    const double measured{
      calc_clad_mainland(
        clad_rate_main,
        n_mainland,
        n_mainland_only,
        carrying_cap_main
      ).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_clad_is)
{
  {
    const per_species_rate clad_rate_is{0.2};
    const int n_species_within_clade_d{5};
    const carrying_capacity carrying_cap_is{50};
    const int n_both{13};
    const double expected{2.34};
    const double measured{calc_glob_clad_island(
      clad_rate_is,
      n_species_within_clade_d,
      carrying_cap_is,
      n_both).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
  {
   const per_species_rate clad_rate_is{0.03};
   const int n_species_within_clade_d{0};
   const carrying_capacity carrying_cap_is{50};
   const int n_both{0};
   const double expected{0.0};
   const double measured{calc_glob_clad_island(
     clad_rate_is,
     n_species_within_clade_d,
     carrying_cap_is,
     n_both).get()
   };
   BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_clad_main)
{
  {
    const per_species_rate clad_rate_main{0.01};
    const int n_both{10};
    const int n_main{250};
    const carrying_capacity carrying_cap_main{500};
    const double expected{0.05};
    const double measured{
      calc_glob_clad_mainland(
        clad_rate_main,
        n_both,
        n_main,
        carrying_cap_main
      ).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  {
    const per_species_rate clad_rate_main{0.1};
    const int n_both{0};
    const int n_main{250};
    const carrying_capacity carrying_cap_main{500};
    const double expected{0.0};
    const double measured{
      calc_glob_clad_mainland(
        clad_rate_main,
        n_both,
        n_main,
        carrying_cap_main
      ).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_iclad)
{
  {
    //Voor iclad:
    //rate * n_island_only * (1- n_species_within_clade / carryingcap),
    // dus in de test 0.02*30*(1-30/60)=0.3
    const per_species_rate rate_clad_is{0.02};
    const int n_island_only{30};
    const int n_species_within_clade_d{30};
    const carrying_capacity carrying_cap_is{60};
    const double expected{0.3};
    const double measured{
      calc_iclad(
        rate_clad_is,
        n_island_only,
        n_species_within_clade_d,
        carrying_cap_is
      ).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
  {
    const per_species_rate rate_clad_is{0.02};
    const int n_island_only{0};
    const int n_species_within_clade_d{0};
    const carrying_capacity carrying_cap_is{60};
    const double expected{0.0};
    const double measured{
      calc_iclad(
        rate_clad_is,
        n_island_only,
        n_species_within_clade_d,
        carrying_cap_is
      ).get()
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_islands_ext_rate_on_island)
{
  const per_species_rate ext_rate_is{0.01};
  const int n_island_only{40};
  const double expected{0.4};
  const double measured{
    calc_islands_ext_rate_on_island(
      ext_rate_is,
      n_island_only
    ).get()
  };
  BOOST_CHECK_CLOSE(expected, measured, 0.001);
}

BOOST_AUTO_TEST_CASE(elly_calc_mainlands_ext_rate_on_mainland)
{
  const per_species_rate ext_rate_main{0.01};
  const int n_mainland_only{400};
  const double expected{4.0};
  const double measured{
    calc_mainlands_ext_rate_on_mainland(
      ext_rate_main,
      n_mainland_only
    ).get()
  };
  BOOST_CHECK_CLOSE(expected, measured, 0.001);
}

BOOST_AUTO_TEST_CASE(elly_calc_migration_to_island)
{
  const per_species_rate mig_rate_main{0.02};
  const int n_species_in_clade_d{5};
  const carrying_capacity carrying_cap_is{30};
  const int n_mainland_species{300};
  const double expected{5.0};
  const double measured{
    calc_migration_to_island(
      mig_rate_main,
      n_species_in_clade_d,
      carrying_cap_is,
      n_mainland_species
    ).get()
  };
  BOOST_CHECK_CLOSE(expected, measured, 0.001);
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_spec_ext_rate_on_mainland)
{
  const per_species_rate ext_rate_main{0.2};
  const int n_both{20};
  const double expected{4.0};
  const double measured{calc_glob_spec_ext_rate_on_mainland(
          ext_rate_main,
          n_both).get()};
  BOOST_CHECK_CLOSE(expected, measured, 0.001);
}

BOOST_AUTO_TEST_CASE(elly_calc_glob_spec_ext_rate_on_island)
{
  const per_species_rate ext_rate_is{0.2};
  const int n_both{20};
  const double expected{4.0};
  const double measured{
    calc_glob_spec_ext_rate_on_island(
      ext_rate_is,
      n_both
    ).get()
  };
  BOOST_CHECK_CLOSE(expected, measured, 0.001);
}

BOOST_AUTO_TEST_CASE(elly_collect_rates)
{
  {
    const parameters p = create_parameters_set1();
    simulation s(p);
    //s.run();
    const event_rates e(p, s);
    const auto m = collect_rates(e);
    BOOST_CHECK_CLOSE((*m.find(event::ana                )).second.get(), e.get_ana().get(),                 0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::clad_glob_on_island)).second.get(), e.get_clad_glob_on_island().get(), 0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::clad_glob_on_main  )).second.get(), e.get_clad_glob_on_main().get(),   0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::clad_island_only   )).second.get(), e.get_clad_island_only().get(),    0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::clad_main_only     )).second.get(), e.get_clad_main_only().get(),      0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::ext_glob_on_island )).second.get(), e.get_ext_glob_on_island().get(),  0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::ext_glob_on_main   )).second.get(), e.get_ext_glob_on_main().get(),    0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::ext_island_only    )).second.get(), e.get_ext_island_only().get(),     0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::ext_main_only      )).second.get(), e.get_ext_main_only().get(),       0.0001);
    BOOST_CHECK_CLOSE((*m.find(event::migration_to_island)).second.get(), e.get_migration_to_island().get(), 0.0001);

  }
}

#pragma GCC diagnostic pop
