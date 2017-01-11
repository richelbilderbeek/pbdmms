#include "daic_run.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_run_r_script)
{
  const std::string r_script_filename{"daic_run_r_script.R"};
  {
    std::ofstream f(r_script_filename);
    f << "print(\"Hello World\")\n";
  }
  run_r_script(r_script_filename);
}

BOOST_AUTO_TEST_CASE(daic_create_script_text)
{
  const std::string text{create_script_text()};
  BOOST_CHECK(!text.empty());
}

BOOST_AUTO_TEST_CASE(daic_run)
{
  const auto in = create_input_article_light();
  const double init_lambda_c{2.5};
  const double init_mu{2.7};
  const int init_k{20};
  const double init_gamma{0.009};
  const double init_lambda_a{1.01};
  const double island_age{4.0};
  const int n_species_main{1000};
  const std::string daisie_input_filename{"daic_run_daisie_input.csv"};
  const std::string daisie_output_filename{"daic_run_daisie_output.csv"};
  const std::string r_script_filename{"daic_run_run_daisie.r"};
  const output out{
    run(
      in,
      init_lambda_c,
      init_mu,
      init_k,
      init_gamma,
      init_lambda_a,
      island_age,
      n_species_main,
      daisie_input_filename,
      daisie_output_filename,
      r_script_filename
    )
  };
  BOOST_CHECK_CLOSE(out.get_lambda_c(), 0.767749320733306, 0.00000000001);
  BOOST_CHECK_CLOSE(out.get_mu(), 1.77166095878803e-09, 0.00000000001);
  BOOST_CHECK_CLOSE(out.get_k(), 3.00003880280079, 0.00000000001);
  BOOST_CHECK_CLOSE(out.get_gamma(), 0.000750314431394791, 0.00000000001);
  BOOST_CHECK_CLOSE(out.get_lambda_a(), 0.000101614571827502, 0.00000000001);
  BOOST_CHECK_CLOSE(out.get_loglik(), -27.3882792747044, 0.00000000001);
  BOOST_CHECK_EQUAL(out.get_df(), 5);
  BOOST_CHECK_EQUAL(out.get_conv(), false);
}

#pragma GCC diagnostic pop


