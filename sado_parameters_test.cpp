#include "sado_parameters.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_parameters_comparisons)
{
  const auto pa = create_article_parameters();
  const auto pg = create_golden_standard_parameters();
  const auto pp = create_profiling_parameters();
  BOOST_CHECK_EQUAL(pa, pa);
  BOOST_CHECK_NE(pa, pg);
  BOOST_CHECK_NE(pa, pp);

  BOOST_CHECK_NE(pg, pa);
  BOOST_CHECK_EQUAL(pg, pg);
  BOOST_CHECK_NE(pg, pp);

  BOOST_CHECK_NE(pg, pa);
  BOOST_CHECK_NE(pp, pg);
  BOOST_CHECK_EQUAL(pp, pp);
}

BOOST_AUTO_TEST_CASE(sado_is_golden_standard)
{
  const auto pa = create_article_parameters();
  const auto pg = create_golden_standard_parameters();
  const auto pp = create_profiling_parameters();
  BOOST_CHECK(!is_golden_standard(pa));
  BOOST_CHECK(is_golden_standard(pg));
  BOOST_CHECK(!is_golden_standard(pp));
}

BOOST_AUTO_TEST_CASE(sado_save_and_read_must_be_symmetrical_article)
{
  const std::string filename{
      "sado_save_and_read_must_be_symmetrical_article.txt"};
  const auto p = create_article_parameters();
  save_parameters(p, filename);
  const auto p_again = read_parameters(filename);
  BOOST_CHECK_EQUAL(p, p_again);
}

BOOST_AUTO_TEST_CASE(sado_save_and_read_must_be_symmetrical_golden_standard)
{
  const std::string filename{
      "sado_save_and_read_must_be_symmetrical_golden_standard.txt"};
  const auto p = create_golden_standard_parameters();
  save_parameters(p, filename);
  const auto p_again = read_parameters(filename);
  BOOST_CHECK_EQUAL(p, p_again);
}

BOOST_AUTO_TEST_CASE(sado_save_and_read_must_be_symmetrical_profiling)
{
  const std::string filename{
      "sado_save_and_read_must_be_symmetrical_profiling.txt"};
  const auto p = create_profiling_parameters();
  save_parameters(p, filename);
  const auto p_again = read_parameters(filename);
  BOOST_CHECK_EQUAL(p, p_again);
}

#pragma GCC diagnostic pop
