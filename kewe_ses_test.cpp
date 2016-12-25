#include "kewe_ses.h"

#include <cassert>
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
#include "kewe_results.h"
#include "kewe_parameters.h"
#include "kewe_simulation_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_test_couple_attractiveness_decides_when_to_mate)
{
  const simulation_parameters p = create_sim_parameters_article_figure_3();
  std::mt19937 gen(p.seed);

  const individual a(1.0, 1.0, 1.0, {1.0}, {1.0}, {1.0});

  BOOST_CHECK( attractive_enough(a, a, p, gen));

  const individual b(9.0, 9.0, 9.0, {9.0}, {9.0}, {9.0});

  BOOST_CHECK(!attractive_enough(a, b, p, gen));
}

BOOST_AUTO_TEST_CASE(kewe_test_couple_fitness_decides_if_able_to_mate)
{
  simulation_parameters p = create_sim_parameters_article_figure_3();
  std::mt19937 gen(p.seed);

  const individual a(p, gen);

  BOOST_CHECK(fitness_high_enough(a, 1.0, a, 1.0, p, gen));

  const individual b(p, gen);

  //p.x0 = -300; //?has nothing to do with the test?
  p.popsize = 2;

  BOOST_CHECK(!fitness_high_enough(a, 1.0, b, 2.0, p, gen));
}

 BOOST_AUTO_TEST_CASE(kewe_diploid_run_from_file)
{
  #ifdef NOT_NOW_TAKES_TOO_LONG_20161220_1454
  //====FIX_ISSUE_131====
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  parameters p = read_parameters("testparameters");

  p.m_sim_parameters.haploid = 0;
  p.m_sim_parameters.diploid = 1;

  simulation s(p);
  s.run();
  #endif // NOT_NOW_TAKES_TOO_LONG_20161220_1454
}

BOOST_AUTO_TEST_CASE(kewe_different_allele_sizes_from_file)
{
  #ifdef NOT_NOW_TAKES_TOO_LONG_20161220_1454
  //====FIX_ISSUE_131====
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  parameters p = read_parameters("testparameters");

  p.m_sim_parameters.Np = 6;
  p.m_sim_parameters.Nq = 2;
  p.m_sim_parameters.Nx = 4;

  simulation s(p);
  s.run();
  #endif // NOT_NOW_TAKES_TOO_LONG_20161220_1454
}

BOOST_AUTO_TEST_CASE(kewe_haploid_run)
{
  const parameters p = create_test_parameters_haploid_1();
  simulation s(p);
  BOOST_CHECK_NO_THROW(s.run());
}

BOOST_AUTO_TEST_CASE(kewe_create_initial_population_creates_slightly_different_individuals)
{
  std::mt19937 gen(42);
  simulation_parameters p = create_sim_parameters_article_figure_3();
  p.popsize = 10;
  const individuals pop = create_initial_population(p, gen);
  BOOST_CHECK(pop[0] != pop[2]);
  BOOST_CHECK(pop[3] != pop[5]);

}

BOOST_AUTO_TEST_CASE(kewe_population_with_two_species_stable)
{
  #ifdef NOT_NOW_20161220_1442
  parameters p;
  p.m_sim_parameters.p0 = -1.0;
  p.m_sim_parameters.q0 = -1.0;
  p.m_sim_parameters.x0 = -1.0;
  \
  parameters q;
  q.m_sim_parameters.p0 = 1.0;
  q.m_sim_parameters.q0 = 1.0;
  q.m_sim_parameters.x0 = 1.0;

  std::mt19937 gen(42);

  p.m_sim_parameters.popsize = 100;
  p.m_sim_parameters.endtime = 100;
  p.m_sim_parameters.sc = 0.4;
  p.m_sim_parameters.se = 0.6;
  p.m_sim_parameters.sm = 0.001;
  p.m_sim_parameters.sk = 1.1;
  individuals pop = create_initial_population(p, gen);
  std::cout << pop[0] << '\n' << pop[99] << '\n';
  simulation s(p);

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;
  result_variables output_variables;
  s.reserve_space_output_vectors(output_variables, histX, histP, histQ, p);

  std::vector<std::pair<bigint, int>> ltt_plot;

  for (unsigned int t = 0; t < p.m_sim_parameters.endtime; ++t)
    {

      pop = create_next_generation(p, pop, gen);
      if(t%5==0)
        output(t,histX,histP,histQ,p,pop,output_variables, ltt_plot);
    }

  genotypes g = calc_average_genotype(pop);
  calculate_rho(pop, g, output_variables);

  BOOST_CHECK(output_variables.m_rhopq.back() >= 0.5);
  #endif // NOT_NOW_20161220_1442

}

BOOST_AUTO_TEST_CASE(kewe_fitness_becomes_higher_in_middle_lower_with_more_competition)
{
  //====FIX_ISSUE_126====
}

BOOST_AUTO_TEST_CASE(kewe_calc_survivability)
{
  //When you are the only one at the optimal trait without competition,
  //survivability is 100%
  {
    const double ecological_trait{0.0};
    const gausser ecological_distribution_width(1.0);
    const double competition_intensity{0.0};
    const int population_size{1};
    const double expected{1.0};
    const double measured{
      calc_survivability(
        ecological_trait,
        ecological_distribution_width,
        competition_intensity,
        population_size
      )
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  //When you are in a population of two in which you have no competition,
  //survivability is 100%
  {
    const double ecological_trait{0.0};
    const gausser ecological_distribution_width(1.0);
    const double competition_intensity{0.0};
    const int population_size{2};
    const double expected{1.0};
    const double measured{
      calc_survivability(
        ecological_trait,
        ecological_distribution_width,
        competition_intensity,
        population_size
      )
    };
    BOOST_CHECK_CLOSE(expected, measured, 0.0001);
  }
  //When you are in a population of two and have some competition,
  //survivability is less 100%
  {
    const double ecological_trait{0.0};
    const gausser ecological_distribution_width(1.0);
    const double competition_intensity{0.5};
    const int population_size{2};
    const double measured{
      calc_survivability(
        ecological_trait,
        ecological_distribution_width,
        competition_intensity,
        population_size
      )
    };
    BOOST_CHECK(measured < 0.9);
  }
}



#pragma GCC diagnostic pop
