#include "daic_input_row.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_input_row_constructor_and_getters_must_match)
{
  const std::string clade_name{"platypuses"};
  const species_status status{species_status::endemic};
  const int n_missing_species{42};
  const std::vector<double> branching_times = {1.2, 0.98, 0.54};

  const input_row r(
    clade_name,
    status,
    n_missing_species,
    branching_times
  );
  BOOST_CHECK(r.get_branching_times() == branching_times);
  BOOST_CHECK_EQUAL(r.get_clade_name(), clade_name);
  BOOST_CHECK_EQUAL(r.get_n_missing_species(), n_missing_species);
  BOOST_CHECK_EQUAL(r.get_status(), status);
}

BOOST_AUTO_TEST_CASE(daic_input_row_constructor_needs_branching_times)
{
  const std::string clade_name{"platypuses"};
  const species_status status{species_status::endemic};
  const int n_missing_species{42};
  const std::vector<double> branching_times = {};

  BOOST_CHECK_THROW(
    input_row(
      clade_name,
      status,
      n_missing_species,
      branching_times
    ), std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(daic_create_input_header_has_three_tabs)
{
  const std::string s = create_input_header();
  BOOST_CHECK_EQUAL(std::count(s.begin(), s.end(), '\t'), 3);
}

BOOST_AUTO_TEST_CASE(daic_input_row_operator_stream_out)
{
  std::stringstream s;
  const input_row r(
    "platypus",
    species_status::endemic,
    0,
    {1.2, 0.98, 0.54}
  );
  s << r;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(daic_input_row_operator_stream_out_has_three_tabs)
{
  std::stringstream s;
  const input_row r(
    "platypus",
    species_status::endemic,
    0,
    {1.2, 0.98, 0.54}
  );
  s << r;
  const std::string t = s.str();
  BOOST_CHECK_EQUAL(std::count(std::begin(t), std::end(t), '\t'), 3);
}

#pragma GCC diagnostic pop
