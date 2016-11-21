#include <fstream>
#include <iostream>
#include <string>
#include <QFile>
#include <boost/algorithm/string/split.hpp>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

/*bool doubles_are_similar_enough(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const double epsilon)
{
  assert(x.size() == y.size());

  for (int i = 5; i < 7; ++i)
    {
      double relativeEpsilon = x[i] * epsilon;
      if (std::abs(x[i] - y[i]) > relativeEpsilon)
        {
          std::cout << x[i] << " and " << y[i] << " are not similar enough.\n";
          return false;

        }
    }

  return true;


}

BOOST_AUTO_TEST_CASE(test_kewe_can_recreate_golden_output)
{
  const std::string golden_output_filename{"golden_output"};
  recreate_golden_output(golden_output_filename);
  BOOST_CHECK(is_regular_file(golden_output_filename));
}

BOOST_AUTO_TEST_CASE(test_kewe_output_similar)
{
  const std::string golden_output_filename{"golden_output"};
  recreate_golden_output(golden_output_filename);
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");
  simulation s(parameters);
  const kewe_parameters paraCheck = s.get_parameters();
  s.run();

  const auto output = file_to_vector("defaultresults");
  const auto expected = file_to_vector(golden_output_filename);
  BOOST_CHECK_EQUAL(output.size(), expected.size());
  assert(output.size() > 0);
  assert(expected.size() > 0);

  const std::vector<std::string> splitOutput = seperate_string(output[0], ',');
  const std::vector<std::string> splitExpected = seperate_string(expected[0], ',');

  assert(splitOutput.size() == splitExpected.size());
  assert(splitOutput.size() > 0);
  assert(splitExpected.size() > 0);


  std::vector<double> d_output;
  std::vector<double> d_expected;
  std::string::size_type sz;

  for (int i = 0; i < static_cast<int>(splitOutput.size()); ++i)
    {
      d_output.push_back(str_to_double(splitOutput[i]));
      d_expected.push_back(str_to_double(splitExpected[i]));
    }

  double relativeEpsilon = 0.15;

  BOOST_CHECK(doubles_are_similar_enough(d_output, d_expected, relativeEpsilon));


  std::clog << std::string(40,'*') << "\n\n";
  std::clog << output[0] << '\n' <<  expected[0] << "\n\n";
  std::clog << std::string(40,'*')<< '\n';
}*/

BOOST_AUTO_TEST_CASE(test_kewe_diploid_run)
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");

  parameters.sim_parameters.haploid = 0;
  parameters.sim_parameters.diploid = 1;

  simulation s(parameters);
  const kewe_parameters paraCheck = s.get_parameters();
  s.run();
}

BOOST_AUTO_TEST_CASE(test_kewe_different_allele_sizes)
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");

  parameters.sim_parameters.Np = 6;
  parameters.sim_parameters.Nq = 2;
  parameters.sim_parameters.Nx = 4;

  simulation s(parameters);
  const kewe_parameters paraCheck = s.get_parameters();
  s.run();
}

BOOST_AUTO_TEST_CASE(test_kewe_diploid_too_few_alleles)
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");

  parameters.sim_parameters.Np = 1;
  parameters.sim_parameters.diploid = 1;
  simulation s(parameters);
  BOOST_CHECK_THROW(s.run(),std::invalid_argument);
  const kewe_parameters paraCheck = s.get_parameters();
}





#pragma GCC diagnostic pop
