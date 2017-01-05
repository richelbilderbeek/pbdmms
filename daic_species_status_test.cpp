#include "daic_species_status.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>
#include <string>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_to_str_and_to_species_status_must_be_symmetrical)
{
  {
    species_status s = species_status::endemic;
    BOOST_CHECK_EQUAL(to_str(s), "Endemic");
  }
  {
    species_status s = species_status::non_endemic;
    BOOST_CHECK_EQUAL(to_str(s), "Non_endemic");
  }
  {
    species_status s = species_status::non_endemic_max_age;
    BOOST_CHECK_EQUAL(to_str(s), "Non_endemic_Max_age");
  }
  {
    std::string str = "Endemic";
    BOOST_CHECK_EQUAL(to_species_status(str), species_status::endemic);
  }
  {
    std::string str = "Non_endemic";
    BOOST_CHECK_EQUAL(to_species_status(str), species_status::non_endemic);
  }
  {
    std::string str = "Non_endemic_Max_age";
    BOOST_CHECK_EQUAL(to_species_status(str), species_status::non_endemic_max_age);
  }
}


#pragma GCC diagnostic pop




