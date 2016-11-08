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

BOOST_AUTO_TEST_CASE(pbd_is_regular_file)
{
  const std::string filename{"pbd_is_regular_file.txt"};
  if (is_regular_file(filename)) { delete_file(filename); }
  assert(!is_regular_file(filename));
  //Create file
  {
    std::fstream f;
    f.open(filename.c_str(),std::ios::out);
    f << "TEMP TEXT";
    f.close();
  }
  BOOST_CHECK(is_regular_file(filename));
  delete_file(filename);
  BOOST_CHECK(!is_regular_file(filename));

}

BOOST_AUTO_TEST_CASE(pbd_run_pbd_sim_should_produce_l_table)
{
  l_table run_pbd_sim(
    const double birth_good,
    const double birth_incipient,
    const double completion,
    const double death_good,
    const double death_incipient,
    const double time,
    const int seed
  );

}

BOOST_AUTO_TEST_CASE(pbd_run_pbd_sim_should_produce_png)
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
  run_pbd_sim(
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

#pragma GCC diagnostic pop



