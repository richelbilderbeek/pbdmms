#include <fstream>
#include <iostream>
#include <string>
#include <QFile>
#include <random>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_results.h"
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
  //====FIX_ISSUE_131====
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
  //====FIX_ISSUE_131====
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

BOOST_AUTO_TEST_CASE(test_kewe_create_initial_population_creates_slightly_different_individuals)
{
  std::mt19937 gen(42);
  kewe_parameters p;
  p.sim_parameters.popsize = 10;
  std::vector<indiv> pop = create_initial_population(p, gen);
  BOOST_CHECK(pop[0] != pop[2]);
  BOOST_CHECK(pop[3] != pop[5]);

}

BOOST_AUTO_TEST_CASE(test_kewe_population_with_two_species_stable)
{
  kewe_parameters p;
  p.sim_parameters.p0 = -1.0;
  p.sim_parameters.q0 = -1.0;
  p.sim_parameters.x0 = -1.0;
  \
  kewe_parameters q;
  q.sim_parameters.p0 = 1.0;
  q.sim_parameters.q0 = 1.0;
  q.sim_parameters.x0 = 1.0;

  std::mt19937 gen(42);

  p.sim_parameters.popsize = 100;
  p.sim_parameters.endtime = 100;
  p.sim_parameters.sc = 0.4;
  p.sim_parameters.se = 0.6;
  p.sim_parameters.sm = 0.001;
  p.sim_parameters.sk = 1.1;
  std::vector<indiv> pop = create_initial_population(p, gen);
  std::cout << pop[0] << '\n' << pop[99] << '\n';
  for(int i = 0; i < 50; ++i)
    pop[i].init(q, gen);
  simulation s(p);

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;
  result_variables output_variables;
  s.reserve_space_output_vectors(output_variables, histX, histP, histQ, p);

  for (unsigned int t = 0; t < p.sim_parameters.endtime; ++t)
    {

      pop = create_next_generation(p, pop, gen);
      if(t%5==0)
        output(t,histX,histP,histQ,p,pop,output_variables);
    }

  genotypes g = calc_average_genotype(pop);
  calculate_rho(pop, g, output_variables);

  BOOST_CHECK(output_variables.m_rhopq.back() >= 0.5);

}

BOOST_AUTO_TEST_CASE(test_kewe_fitness_becomes_higher_in_middle_lower_with_more_competition)
{
  //====FIX_ISSUE_126====
}

#pragma GCC diagnostic pop
