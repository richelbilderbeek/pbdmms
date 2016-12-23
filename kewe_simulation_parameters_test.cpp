#include "kewe_simulation_parameters.h"

#include <cmath>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_sim_parameters_set_and_get_must_be_symmetrical)
{
  {
    simulation_parameters p = create_sim_parameters_branching();
    const int t_end{p.get_end_time() + 1};
    p.set_end_time(t_end);
    BOOST_CHECK_EQUAL(p.get_end_time(), t_end);
  }
  {
    simulation_parameters p = create_sim_parameters_branching();
    const double sigma_k{p.get_eco_res_distribution_width() + 1.0};
    p.set_eco_res_distribution_width(sigma_k);
    BOOST_CHECK_EQUAL(p.get_eco_res_distribution_width(), sigma_k);
  }
  {
    simulation_parameters p = create_sim_parameters_branching();
    const double sigma_v{p.get_mut_distr_width() + 1.0};
    p.set_mut_distr_width(sigma_v);
    BOOST_CHECK_EQUAL(p.get_mut_distr_width(), sigma_v);
  }
  {
    simulation_parameters p = create_sim_parameters_branching();
    const double sigma_c{p.get_eco_res_util_width() + 1.0};
    p.set_eco_res_util_width(sigma_c);
    BOOST_CHECK_EQUAL(p.get_eco_res_util_width(), sigma_c);

  }
}


BOOST_AUTO_TEST_CASE(kewe_create_sim_parameters_branching)
{
  const auto p = create_sim_parameters_branching();
  BOOST_CHECK(will_branch_on_ecotype(p));
  BOOST_CHECK(will_branch_on_male_mating_type(p));
  BOOST_CHECK(will_give_sympatric_speciation(p));
}

BOOST_AUTO_TEST_CASE(kewe_create_sim_parameters_article_figure_3)
{
  const auto p = create_sim_parameters_article_figure_3();
  //Use style that is easy to check
  BOOST_CHECK_CLOSE(get_sigma_c(p), 0.4 , 0.0001);
  BOOST_CHECK_CLOSE(get_sigma_k(p), 1.2 , 0.0001);
  BOOST_CHECK_CLOSE(get_sigma_e(p), 0.6 , 0.0001);
  BOOST_CHECK_CLOSE(get_sigma_m(p), 0.2 , 0.0001);
  BOOST_CHECK_CLOSE(get_sigma_v(p), 0.02, 0.0001);
  BOOST_CHECK_CLOSE(get_sigma_s(p), 1.0 , 0.0001);
  //BOOST_CHECK_CLOSE(get_eta(p) , 1.0, 0.0001); //eta is assumed zero

  //BOOST_CHECK(will_branch_on_ecotype(p));
  //BOOST_CHECK(will_branch_on_male_mating_type(p));
  //BOOST_CHECK(will_give_symatric_speciation(p));
}

BOOST_AUTO_TEST_CASE(kewe_simulation_parameters_getters)
{
  const auto p = create_sim_parameters_article_figure_3();
  BOOST_CHECK_CLOSE(get_sigma_c(p), p.get_eco_res_util_width(),  0.0001);
  BOOST_CHECK_CLOSE(get_sigma_k(p), p.get_eco_res_distribution_width(),  0.0001);
  BOOST_CHECK_CLOSE(get_sigma_e(p), p.get_mate_spec_eco(),  0.0001);
  BOOST_CHECK_CLOSE(get_sigma_m(p), p.get_mate_spec_mate(),  0.0001);
  BOOST_CHECK_CLOSE(get_sigma_v(p), p.get_mut_distr_width(),  0.0001);
  BOOST_CHECK_CLOSE(get_sigma_s(p), p.get_viab_sel_male_mate_str(),  0.0001);

  //BOOST_CHECK(will_branch_on_ecotype(p));
  //BOOST_CHECK(will_branch_on_male_mating_type(p));
  //BOOST_CHECK(will_give_symatric_speciation(p));
}

BOOST_AUTO_TEST_CASE(kewe_create_sim_parameters_random)
{
  const auto a = create_sim_parameters_random();
  const auto b = create_sim_parameters_random();
  //STUB
  BOOST_CHECK_NE(a.get_mate_spec_eco(), b.get_mate_spec_eco());
  BOOST_CHECK_NE(a.get_mate_spec_mate(), b.get_mate_spec_mate());
}


#pragma GCC diagnostic pop
