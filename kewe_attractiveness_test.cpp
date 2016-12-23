#include "kewe_attractiveness.h"

#include <cmath>

#include "kewe_helper.h"
#include "kewe_parameters.h"
#include "kewe_individual.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_calculate_attractiveness)
{
  {
    const double female_preference{1.0};
    const double male_trait{1.0};
    const double mate_spec_mate{0.1};
    const double female_ecotype{1.0};
    const double male_ecotype{1.0};
    const double mate_spec_eco{0.1};
    const double measured{
      calc_attractiveness(
        female_preference,
        male_trait,
        mate_spec_mate,
        female_ecotype,
        male_ecotype,
        mate_spec_eco
      )
    };
    const double expected{1.0};
    BOOST_CHECK_CLOSE(measured, expected, 0.001);
  }
  {
    //male sexual trait and female sexual preference differ two standard deviations
    const double female_preference{1.0};
    const double male_trait{0.0};
    const double mate_spec_mate{0.5};
    const double female_ecotype{1.0};
    const double male_ecotype{1.0};
    const double mate_spec_eco{0.1};
    const double measured{
      calc_attractiveness(
        female_preference,
        male_trait,
        mate_spec_mate,
        female_ecotype,
        male_ecotype,
        mate_spec_eco
      )
    };
    const double expected{gauss(1.0, 0.5)};
    BOOST_CHECK_CLOSE(measured, expected, 0.001);
  }
  {
    //Both:
    // * male sexual trait and female sexual preference differ three standard deviations
    // * male ecological trait and female ecological trait differ five standard deviations
    const double female_preference{-2.0};
    const double male_trait{-1.0};
    const double mate_spec_mate{0.333};
    const double female_ecotype{1.0};
    const double male_ecotype{2.0};
    const double mate_spec_eco{0.2};
    const double measured{
      calc_attractiveness(
        female_preference,
        male_trait,
        mate_spec_mate,
        female_ecotype,
        male_ecotype,
        mate_spec_eco
      )
    };
    const double expected{gauss(1.0, 0.333) * gauss(1.0, 0.2)};
    BOOST_CHECK_CLOSE(measured, expected, 0.001);
  }
  {
    //male ecological trait and female ecological trait differ two standard deviations
    const double female_preference{1.0};
    const double male_trait{1.0};
    const double mate_spec_mate{0.1};
    const double female_ecotype{1.0};
    const double male_ecotype{0.0};
    const double mate_spec_eco{0.5};
    const double measured{
      calc_attractiveness(
        female_preference,
        male_trait,
        mate_spec_mate,
        female_ecotype,
        male_ecotype,
        mate_spec_eco
      )
    };
    const double expected{gauss(1.0, 0.5)};
    BOOST_CHECK_CLOSE(measured, expected, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(kewe_similar_individuals_attractiveness_is_high)
{
  const individual a;
  const individual b;
  BOOST_CHECK(a == b);

  const simulation_parameters p = create_sim_parameters_article_figure_3();

  const attractiveness at{calc_attractiveness(a, b, p)};

  BOOST_CHECK(at > 0.9);
}

BOOST_AUTO_TEST_CASE(kewe_different_individuals_attractiveness_is_low)
{
  simulation_parameters parameters_a;

  std::mt19937 gen(parameters_a.seed);

  const individual a(parameters_a, gen);
  simulation_parameters parameters_b;
  parameters_b.q0 = -0.5;
  const individual b(parameters_b, gen);

  BOOST_CHECK(a != b);

  const attractiveness at{calc_attractiveness(a, b, parameters_a)};

  BOOST_CHECK(at < 0.1);
}

#pragma GCC diagnostic pop
