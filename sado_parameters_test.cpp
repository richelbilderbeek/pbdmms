#include "sado_parameters.h"

#include <fstream>

#include <boost/test/unit_test.hpp>

#include "sado_helper.h"

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_parameters_comparisons)
{
  const auto pa = create_article_parameters();
  const auto pg = create_golden_standard_parameters();
  const auto pp = create_profiling_parameters();
  BOOST_CHECK_EQUAL(pa, pa);
  BOOST_CHECK_NE(pa, pg);
  BOOST_CHECK_NE(pa, pp);
  BOOST_CHECK(pa != pp);

  BOOST_CHECK_NE(pg, pa);
  BOOST_CHECK_EQUAL(pg, pg);
  BOOST_CHECK_NE(pg, pp);

  BOOST_CHECK_NE(pg, pa);
  BOOST_CHECK_NE(pp, pg);
  BOOST_CHECK_EQUAL(pp, pp);
}

BOOST_AUTO_TEST_CASE(sado_parameters_testrun)
{
  const auto p = create_testrun_parameters();
}

BOOST_AUTO_TEST_CASE(sado_read_parameters_should_throw_when_no_parameter_file)
{
  const std::string filename{
    "sado_read_parameters_should_throw_when_no_parameter_file"
  };
  assert(!is_regular_file(filename));
  BOOST_CHECK_THROW(read_parameters(filename), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(sado_read_parameters_should_throw_when_invalid_parameter_file)
{
  const std::string filename{
    "sado_read_parameters_should_throw_when_invalid_parameter_file.txt"
  };
  std::ofstream f(filename);
  f << "alleles 2 2 2\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "histbin 0.1 0.1 0.1 0.1\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "seed 123\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "pop0 100\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "type0 0.5 0.5 0.5\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "end 4000\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "sc 0.3\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "se 0.1\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "sk 1.2\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "c 0.01\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "sm 0.1\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "sv 0.02\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "sq 1.0\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "eta 1.0\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "b 4.0\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "output 10 output.txt\n";
  BOOST_CHECK_THROW(read_parameters(filename), std::runtime_error);
  f << "at 0.05\n";
  delete_file(filename);
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
  delete_file(filename);
}

BOOST_AUTO_TEST_CASE(sado_save_and_read_must_be_symmetrical_golden_standard)
{
  const std::string filename{
      "sado_save_and_read_must_be_symmetrical_golden_standard.txt"};
  const auto p = create_golden_standard_parameters();
  save_parameters(p, filename);
  const auto p_again = read_parameters(filename);
  BOOST_CHECK_EQUAL(p, p_again);
  delete_file(filename);
}

BOOST_AUTO_TEST_CASE(sado_save_and_read_must_be_symmetrical_profiling)
{
  const std::string filename{
      "sado_save_and_read_must_be_symmetrical_profiling.txt"};
  const auto p = create_profiling_parameters();
  save_parameters(p, filename);
  const auto p_again = read_parameters(filename);
  BOOST_CHECK_EQUAL(p, p_again);
  delete_file(filename);
}
