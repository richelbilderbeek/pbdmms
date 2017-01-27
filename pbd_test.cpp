#include "pbd_helper.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "pbd_l_table.h"
#include "pbd.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_sim_to_l_table_should_produce_l_table)
{
  const double birth_good{0.2};
  const double birth_incipient{0.2};
  const double completion{0.2};
  const double death_good{0.1};
  const double death_incipient{0.1};
  const double time{0.2};
  const int seed{42};

  const l_table result{
    sim_to_l_table(
      birth_good,
      birth_incipient,
      completion,
      death_good,
      death_incipient,
      time,
      seed
    )
  };
  //PBD::pbd_sim always produces a phylogeny with two taxa
  BOOST_CHECK(result.size() >= 2);
}

BOOST_AUTO_TEST_CASE(pbd_sim_to_nltt_recon_should_produce_nltt)
{
  const double birth_good{0.2};
  const double birth_incipient{0.2};
  const double completion{0.2};
  const double death_good{0.1};
  const double death_incipient{0.1};
  const double time{0.2};
  const int seed{42};

  const auto result = sim_to_nltt_recon(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
  //An nLTT plot always has at least two timepoints
  BOOST_CHECK(result.size() >= 2);
}

BOOST_AUTO_TEST_CASE(pbd_sim_to_nltt_igtree_extinct_should_produce_nltt)
{
  const double birth_good{0.2};
  const double birth_incipient{0.2};
  const double completion{0.2};
  const double death_good{0.1};
  const double death_incipient{0.1};
  const double time{0.2};
  const int seed{42};
  const pbd::parameters p(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
  const auto result = sim_to_nltt_igtree_extinct(p);
  //An nLTT plot always has at least two timepoints
  BOOST_CHECK(result.size() >= 2);
}


BOOST_AUTO_TEST_CASE(pbd_sim_to_png_should_produce_png)
{
  const std::string png_filename{
    "pbd_run_pbd_sim_should_produce_png.png"
  };

  if (is_regular_file(png_filename)) delete_file(png_filename);
  assert(!is_regular_file(png_filename));

  const double birth_good{0.2};
  const double birth_incipient{0.2};
  const double completion{0.2};
  const double death_good{0.1};
  const double death_incipient{0.1};
  const double time{0.2};
  const int seed{42};
  sim_to_png(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed,
    png_filename
  );
  BOOST_CHECK(is_regular_file(png_filename));
}

BOOST_AUTO_TEST_CASE(test_pbd_only_sim_to_nltt_recon_should_produce_a_file)
{
  const parameters p = create_parameters_set1();

  //Ensure there is no output file yet
  const std::string filename = "only_sim_to_nltt_recon.csv";
  if (is_regular_file(filename))
  {
    delete_file(filename);
  }
  assert(!is_regular_file(filename));
  const parameters a = create_parameters_set1();
  pbd::only_sim_to_nltt_recon(a);

  BOOST_CHECK(is_regular_file(filename));

  //Clean up
  delete_file(filename);
}

#pragma GCC diagnostic pop



