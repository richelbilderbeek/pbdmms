#include <fstream>
#include <iostream>
#include <string>
#include <QFile>
#include <random>
#include <boost/algorithm/string/split.hpp>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(kewe_test_couple_attractiveness_decides_when_to_mate)
{

  kewe_parameters parameters;
  std::mt19937 gen(parameters.sim_parameters.seed);

  indiv a(parameters);
  a.init(parameters, gen);

  BOOST_CHECK(attractive_enough(a, a, parameters, gen));

  indiv b(parameters);

  parameters.sim_parameters.q0 = -300;
  b.init(parameters, gen);

  BOOST_CHECK(!attractive_enough(a, b, parameters, gen));
}

BOOST_AUTO_TEST_CASE(kewe_test_couple_fitness_decides_if_able_to_mate)
{
  kewe_parameters parameters;
  std::mt19937 gen(parameters.sim_parameters.seed);

  indiv a(parameters);
  a.init(parameters, gen);

  BOOST_CHECK(fitness_high_enough(a, 1.0, a, 1.0, parameters, gen));

  indiv b(parameters);

  parameters.sim_parameters.x0 = -300;
  parameters.sim_parameters.popsize = 2;
  b.init(parameters, gen);

  BOOST_CHECK(!fitness_high_enough(a, 1.0, b, 2.0, parameters, gen));
}

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

BOOST_AUTO_TEST_CASE(test_kewe_similar_individuals_attractiveness_is_high)
{
  kewe_parameters parameters;

  indiv a(parameters);
  indiv b(parameters);

  BOOST_CHECK(a == b);

  double attractiveness = calc_attractiveness(a, b, parameters);

  BOOST_CHECK(attractiveness > 0.9);
}

BOOST_AUTO_TEST_CASE(test_kewe_different_individuals_attractiveness_is_low)
{
  kewe_parameters parameters_a;

  std::mt19937 gen(parameters_a.sim_parameters.seed);

  indiv a(parameters_a);
  a.init(parameters_a, gen);
  kewe_parameters parameters_b;
  parameters_b.sim_parameters.q0 = -0.5;
  indiv b(parameters_b);
  b.init(parameters_b, gen);

  BOOST_CHECK(a != b);

  double attractiveness = calc_attractiveness(a, b, parameters_a);

  BOOST_CHECK(attractiveness < 0.1);
}



#pragma GCC diagnostic pop
