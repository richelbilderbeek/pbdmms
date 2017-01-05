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
    const std::string str = "Non_endemic";
    BOOST_CHECK_EQUAL(to_species_status(str), species_status::non_endemic);
  }
  {
    const std::string str = "Non_endemic_Max_age";
    BOOST_CHECK_EQUAL(to_species_status(str), species_status::non_endemic_max_age);
  }
  {
    const species_status s = species_status::non_endemic_max_age;
    const std::string t = to_str(s);
    BOOST_CHECK_EQUAL(s, to_species_status(t));
  }
  {
    const std::string s = "nonsense";
    BOOST_CHECK_THROW(to_species_status(s), std::invalid_argument);
  }
}


#pragma GCC diagnostic pop




